#include <gst/gst.h>

int main ( int argc, char** argv)
{
	//GstElement *pipe;
	GstElement *playbin;

	gst_init (&argc, &argv);

	//pipe = gst_parse_launch ("playbin uri=file:///home/binli/test.mkv", NULL);
	playbin = gst_element_factory_make ("playbin", NULL);
	g_object_set (playbin, "uri", "file:///home/binli/Downloads/2016-04-14-Canonical-Town-Hall.mp4", NULL);

	//gst_element_set_state (pipe, GST_STATE_PLAYING);
	gst_element_set_state (playbin, GST_STATE_PLAYING);

	do {} while (1);

	return 0;
}
