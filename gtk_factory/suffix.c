#include <glib.h>

// gcc -o simple simple.c `pkg-config --libs --cflags glib-2.0` 
int main(int argc, char *argv[])
{
	/* g_str_has_suffix() */
	/* Check if a string has a particular suffix.*/
	gboolean suffix_value;

	suffix_value = g_str_has_suffix("ftp://www.example.com/packages/example-1.0-1.tar.gz.distrib", ".tar.gz");

	if(suffix_value == TRUE) {
		g_print("Suffix is true \n");
	}
	else {
		g_print("Suffix is false \n");
	}

	return 0;
}
