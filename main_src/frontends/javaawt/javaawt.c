/*

    arfminesweeper: Cross-plataform multi-frontend game
    Copyright (C) 2023 arf20 (Ángel Ruiz Fernandez)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    java.c: Java frontend

*/

#include <stdlib.h>
#include <dlfcn.h>

#include <jni.h>

#include <common/frontconf.h>

#include "javaawt.h"

JavaVM *jvm;       /* denotes a Java VM */

static const int *board = NULL;
static int size = 0;

int javaawt_start(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    /* sufficient to link the bindings to this process */
    void *bindingshandle = dlopen(LIB_BINDINGS_JAVA, RTLD_NOW);
    if (bindingshandle == NULL) {
        fprintf(stderr, "Error: loading %s\n", dlerror());
        return -1;
    }

    JNIEnv *env;       /* pointer to native method interface */
    JavaVMInitArgs vm_args; /* JDK/JRE 6 VM initialization arguments */
    JavaVMOption options[3];
    options[0].optionString = "-Djava.class.path=" JAVA_CLASS_PATH;
    options[1].optionString = "-Djava.library.path=" JAVA_LIBRARY_PATH;
    options[2].optionString = "-Xlog:library=info"; /* for debug */
    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = 2;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = 0;

    /* load and initialize a Java VM, return a JNI interface
     * pointer in env */
    jint ret = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
    if (ret != JNI_OK)  {
        printf("Error JNI_CreateJavaVM\n");
        return -1;
    }

    /* invoke the Main.test method using the JNI */
    jclass class = (*env)->FindClass(env, "awt");
    if (class == NULL)  {
        printf("Error FindClass failed: not found\n");
        return -1;
    }
    jmethodID mid = (*env)->GetStaticMethodID(env, class, "main", "(I)V");
    if (mid == NULL)  {
        printf("Error GetStaticMethodID failed, wrong signature?\n");
        return -1;
    }
    
    (*env)->CallStaticIntMethod(env, class, mid, lsize);

    jthrowable flag = (*env)->ExceptionOccurred(env);
    if (flag) {
        printf("Unhandled Java exception from JavaVM:\n");
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);
        return -1;
    }

}

void javaawt_destroy() {
    (*jvm)->DestroyJavaVM(jvm);
}

const char *
javaawt_name() {
    return "javaawt";
}

