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

    module.c: Linux kernel module entry point

*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("arf20");
MODULE_DESCRIPTION("arfminesweeper linux kernel module");
MODULE_VERSION("0.1");

static int __init
arfmm_start(void) {
    printk(KERN_INFO "Hello world\n");
    return 0;
}

static void __exit
arfmm_end(void) {
    printk(KERN_INFO "Goodbye world\n");
}

module_init(arfmm_start);

module_exit(arfmm_end);
