const Gio = imports.gi.Gio;
const GLib = imports.gi.GLib;
const Lang = imports.lang;
const Gtk = imports.gi.Gtk;
const Shell = imports.gi.Shell;

/*
function dir_statistic (result)
{
    let filename;
    let number = 0;

    let iter = file_dir.enumerate_children_finish(path); // FileEnumerator

    let single = iter.next_file(); // FileInfo

    while (single != null) {

        filename = single.get_name();
        number++;
        log (filename, number);

        single = iter.next_file();
    }
}

function files_num_async(path) 
{
    file_dir = file_new_for_path(path); // File

    file_dir.enumerate_children_async (null, none, 0, null, dir_statistic, null);
}

*/

function listDir(file) {
    let allFiles = [];
    let number = 0;
    let filename;
    let enumerator = file.enumerate_children(Gio.FILE_ATTRIBUTE_STANDARD_NAME,
                            Gio.FileQueryInfoFlags.NONE,
                            null,
                            null);
    let single_file = enumerator.next_file(null);
    while (single_file != null) {
        filename = single_file.get_name();
        number++;
        log (filename);
        log (number);
        single_file = enumerator.next_file(null);
    }
}
function listDirAsync(file, callback) {
    let allFiles = [];
    file.enumerate_children_async(Gio.FILE_ATTRIBUTE_STANDARD_NAME,
                                  Gio.FileQueryInfoFlags.NONE,
                                  GLib.PRIORITY_LOW,
                                  null,
                                  Lang.bind(this,function (obj, res) {
        let enumerator = obj.enumerate_children_finish(res);
        function onNextFileComplete(obj, res) {
            let files = obj.next_files_finish(res);
            if (files.length) {
                allFiles = allFiles.concat(files);
                enumerator.next_files_async(100, GLib.PRIORITY_LOW, null, onNextFileComplete);
            } else {
                enumerator.close(null);
                callback(allFiles);
            }
        }
        enumerator.next_files_async(100, GLib.PRIORITY_LOW, null, onNextFileComplete);
    }));
}

function _onChanged(m, f, of, type) {
    if (type == Gio.FileMonitorEvent.CREATED) {
        log (type + "new file " + f.get_path());
    } else if (type == Gio.FileMonitorEvent.DELETED) {
        log (type + "delete file " + f.get_path());
    } else {
        log (type + "unknow operation " + f.get_path());
    }
    if (type == Gio.FileMonitorEvent.CREATED || type == Gio.FileMonitorEvent.DELETED) {
            listDirAsync(filepath, Lang.bind(this, function (files){
                    log (files.length);
            }));
    }
}

Gtk.init(null,0);

let _paths = GLib.getenv('PATH').split(':');
log("_paths length" + _paths.length);

let filepath = Gio.File.new_for_path("/tmp/abc");

let _appSys = Shell.AppSystem.get_default();
let _gsmPrefs = _appSys.lookup_app('mutt.desktop');
_gsmPrefs.activate();
//listDirAsync(filepath, Lang.bind(this, function (files){
//    log (files.length);
//    for (let i = 0; i < files.length; i++) {
//        log (files[i].get_name());
//    }
//}));
listDir(filepath);
//let monitor = filepath.monitor_directory(Gio.FileMonitorFlags.NONE, null);
//monitor.connect('changed', Lang.bind(this, _onChanged));

Gtk.main();

