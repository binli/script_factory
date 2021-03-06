#include <gio/gio.h>
 
static void
on_plugin_info_read (GFileEnumerator *enumerator,
                   GAsyncResult *result,
                   gpointer *user_data)
{
        GList *file_list, *node;
        GError *error = NULL;
        int number = 0;

        file_list = g_file_enumerator_next_files_finish (enumerator,
                        result, &error);
        node = file_list;
        while (node != NULL) {
                GFileInfo *info;
                GList *next_node;

                next_node = node->next;
                info = (GFileInfo *) node->data;
                g_print ("%s\n", g_file_info_get_name (info));

                node = next_node;
                number++;
        }
        g_print ("totally %d\n\n", number);

        g_list_free (file_list);
}

static void
on_plugin_dir_opened (GFile *plugin_dir_file,
                GAsyncResult *result,
                gpointer *user_data)
{
    GFileEnumerator *enumerator;
    GError *error = NULL;

    enumerator = g_file_enumerate_children_finish(plugin_dir_file, result, &error);
    if (enumerator == NULL) {
            g_print ("enumerator is error\n");
            return;
    }

    g_file_enumerator_next_files_async(enumerator,
                    G_MAXINT,
                    G_PRIORITY_DEFAULT,
                    NULL,
                    (GAsyncReadyCallback) on_plugin_info_read,
                    NULL);

    if (enumerator != NULL)
            g_object_unref(enumerator);
}

static void
dir_is_changed (GFileMonitor *monitor,
                GFile *file,
                GFile *other,
                GFileMonitorEvent event_type,
                gpointer user_data)
{
    if (event_type == G_FILE_MONITOR_EVENT_CREATED)
        g_print ("You just created '%s'!\n", g_file_get_path (file));
    else if (event_type == G_FILE_MONITOR_EVENT_DELETED)
        g_print ("You just deleted '%s'!\n", g_file_get_path (file));
    else if (event_type == G_FILE_MONITOR_EVENT_MOVED)
        g_print ("You just moved '%s'!\n", g_file_get_path (file));

    GFile *file_dir = g_file_new_for_path ("/tmp/abc");

    g_file_enumerate_children_async (file_dir, NULL, G_FILE_QUERY_INFO_NONE, G_PRIORITY_DEFAULT, NULL, (GAsyncReadyCallback)on_plugin_dir_opened, NULL);
}
 
int
main (int argc, char **argv)
{
    GFileInfo *child = NULL;
    GFileEnumerator *iter = NULL;

    g_type_init ();

    GMainLoop *main_loop = g_main_loop_new (NULL, TRUE);


    GFile *file = g_file_new_for_path ("/tmp/abc");
    GFileMonitor * monitor = g_file_monitor_directory (file,
                    G_FILE_MONITOR_NONE,
                    NULL,
                    NULL);

    iter = g_file_enumerate_children (file, NULL, G_FILE_QUERY_INFO_NONE, NULL, NULL);
    if (iter != NULL) {
        child = g_file_enumerator_next_file (iter, NULL, NULL);
        if (child != NULL) {
            do {
                g_print ("%s\n", g_file_info_get_name (child));

                child = g_file_enumerator_next_file (iter, NULL, NULL);
            } while (child != NULL);
        }
    }

    g_signal_connect (monitor, "changed", (GCallback)dir_is_changed, NULL);

    g_main_loop_run (main_loop);

    g_object_unref (file);
    g_main_loop_unref (main_loop);

    return 0;
}
