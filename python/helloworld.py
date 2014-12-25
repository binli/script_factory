#!/usr/bin/env python

import gtk

class Rules(object):
    def __init__(self):
        self.cond = []
        self.act = []

class HelloWorld:

    def hello(self, widget, data=None):
        print "Hello World"
        package_list_file="./debian.list"
        arr = []
        rules = None
        with open(package_list_file, 'r') as plf:
            lines = plf.readlines()
            for line in lines:
                line = line.strip('\n')
                if line.startswith("##"):
                    if rules == None:
                        print "Begin new rules is:" + line
                    elif line.startswith("## End:"):
                        print "End all rules!"
                        arr.append(rules)
                        plf.close()        
                        for a in arr:
                            print a.cond + a.act
                        return
                    else:
                        print "Finish previous rules is:" + line
                        arr.append(rules)
                        rules = None
                    rules = Rules()
                    rules.cond.append(line)
                elif line == '':
                    print "blank line"
                    continue

                elif line.startswith("#"):
                    print "comment line is:" + line
                    continue
                elif line.endswith("-"):
                    print "remove line is:" + line
                    rules.act.append(line)
                else:
                    print "install line is:" + line
                    rules.act.append(line)

    def delete_event(self, widget, event, data=None):
        print "delete event occurred"
        return False

    def destroy(self, widget, data=None):
        print "destroy signal occurred"
        gtk.main_quit()

    def __init__(self):
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
    
        self.window.connect("delete_event", self.delete_event)
    
        self.window.connect("destroy", self.destroy)
    
        self.window.set_border_width(10)
    
        self.button = gtk.Button("Hello World")
    
        self.button.connect("clicked", self.hello, None)
    
        self.button.connect_object("clicked", gtk.Widget.destroy, self.window)
    
        self.window.add(self.button)
    
        self.button.show()
    
        self.window.show()

    def main(self):
        gtk.main()

if __name__ == "__main__":
    hello = HelloWorld()
    hello.main()
