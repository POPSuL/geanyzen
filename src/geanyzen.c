/*
 * geanyzen.c
 *
 * Copyright (c) 2011 Viktor Suprun <popsul1993@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <geany/geanyplugin.h>
#include <gtk-2.0/gdk/gdkkeysyms.h>
#include <python2.6/Python.h>

//gcc -c geanyzen.c -fPIC `pkg-config --cflags geany` && gcc geanyzen.o -o libgeanyzen.so -shared `pkg-config --libs geany` -lpython2.6 && cp -f libgeanyzen.so ~/.config/geany/plugins/


GeanyPlugin         *geany_plugin;
GeanyData           *geany_data;
GeanyFunctions      *geany_functions;

PyObject *pName, *pValue, *main_module, *expand, *getpos;

PLUGIN_VERSION_CHECK(147)

PLUGIN_SET_INFO("Zen-Coding", "Zen-Coding for Geany",
                "0.3.1", "POPSuL <popsul1993@gmail.com>");

enum {
    EXPAND_KB,
    COUNT_KB
};

PLUGIN_KEY_GROUP(zen_group, COUNT_KB)

/*
static int lastPos(char *in, char x, int offset) {
    if(offset == -1)
        offset = strlen(in);
    else
        offset = strlen(in) - offset;
    char *i = in;
    while(offset--) {
        if(*(i + offset) == x)
            return offset;
    }
    return -1;
}
*/

static void expand_handler(G_GNUC_UNUSED guint key_id) {
    GeanyDocument *doc = document_get_current();
    int currl = (int)sci_get_current_line(doc->editor->sci);
    int startpos = sci_get_position_from_line(doc->editor->sci, currl);
    char *cline = (char*)sci_get_line(doc->editor->sci, currl);
    int ccpos = sci_get_current_position(doc->editor->sci);
    int cpos = ccpos - startpos;
    int nlen = strlen(cline);
    char *mime = doc->file_type->mime_type;
    if(nlen > 0) {
        pName = PyTuple_New(3);
        PyObject *line = PyString_FromString(cline);
        PyObject *pos = PyInt_FromLong((long) cpos);
        PyObject *type = PyString_FromString(mime);
        PyTuple_SetItem(pName, 0, line);
        PyTuple_SetItem(pName, 1, pos);
        PyTuple_SetItem(pName, 2, type);

        pValue = PyObject_CallObject(expand, pName);
        char *result = PyString_AsString(pValue);
        pName = PyTuple_New(0);
        pValue = PyObject_CallObject(getpos, pName);
        int ncpos = (int) PyLong_AsLong(pValue);
        if(strlen(result) > 0) {
            int endpos = startpos + nlen;

            sci_set_selection_start(doc->editor->sci, startpos);
            sci_set_selection_end(doc->editor->sci, endpos);
            sci_replace_sel(doc->editor->sci, result);

            if(ncpos != -1) {
                endpos = startpos + ncpos;
            }
            sci_set_current_position(doc->editor->sci, endpos, TRUE);
        } else {
            printf("error result, %s", result);
        }
        Py_DECREF(pName);
        Py_DECREF(pValue);
    }
}



void plugin_init(G_GNUC_UNUSED GeanyData *data)
{
    Py_Initialize();
    PyRun_SimpleString("import os, sys");
    PyRun_SimpleString("sys.path.append(r'/usr/lib/geany-plugins/geanyzen')");
    PyRun_SimpleString("from zencoding.geany_zencoding import expand, getpos");
    main_module = PyImport_AddModule("__main__");
    expand = PyObject_GetAttrString(main_module, "expand");
    getpos = PyObject_GetAttrString(main_module, "getpos");

    keybindings_set_item(plugin_key_group, EXPAND_KB, expand_handler, GDK_e, GDK_CONTROL_MASK, "expand", "expand", NULL  );
}

void plugin_cleanup(void)
{
    Py_DECREF(main_module);
    Py_DECREF(expand);
    Py_DECREF(getpos);
    Py_Finalize();
}
