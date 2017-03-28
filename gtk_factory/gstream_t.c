#include <gst/gst.h>

static void
pipeline_add_video_sink (GstElement * pipeline, GstPad * srcpad)
{
	GstElement *q, *s;
	GstPad *sinkpad;

	q = gst_element_factory_make ("queue", NULL);
	s = gst_element_factory_make ("autovideosink", NULL);
	gst_bin_add_many (GST_BIN (pipeline), q, s, NULL);

	sinkpad = gst_element_get_static_pad (q, "sink");
	gst_pad_link (srcpad, sinkpad)
	gst_element_link (q, s);

	gst_element_sync_state_with_parent (q);
	gst_element_sync_state_with_parent (s);
	gst_object_unref (sinkpad);
}

int main ( int argc, char** argv)
{
	//GstElement *pipe;
	GstElement *pipeline, *d;

	gst_init (&argc, &argv);

	//pipe = gst_parse_launch ("playbin uri=file:///home/binli/test.mkv", NULL);
	d = gst_element_factory_make ("uridecodebin", NULL);
	g_object_set (d, "uri", "file:///home/binli/Downloads/2016-04-14-Canonical-Town-Hall.mp4", NULL);

	pipeline = gst_pipeline_new (NULL);

	gst_bin_add (GST_BIN (pipeline), d);

	g_signal_connect ();
	//gst_element_set_state (pipe, GST_STATE_PLAYING);
	gst_element_set_state (playbin, GST_STATE_PLAYING);

	do {} while (1);

	return 0;
}
