const Gtk = imports.gi.Gtk;
const GLib = imports.gi.GLib;


Gtk.init(null,0);

let mwindow = new Gtk.Window({type : Gtk.WindowType.TOPLEVEL});
let label = new Gtk.Label({label : "Hello"});

mwindow.title = "Bin's gjs";
mwindow.connect ("destroy", function(){Gtk.main_quit()});

mwindow.add (label);

label.show ();
mwindow.show ();

Gtk.main();
