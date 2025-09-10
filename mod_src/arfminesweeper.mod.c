#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x92997ed8, "_printk" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xa916b694, "strnlen" },
	{ 0xc8cf812f, "device_create" },
	{ 0x69b07fa1, "class_create" },
	{ 0x9e4f6a2f, "class_unregister" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0x75ca79b5, "__fortify_panic" },
	{ 0xfb578fc5, "memset" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x2a8af3ef, "__register_chrdev" },
	{ 0x281722e1, "device_destroy" },
	{ 0x41ed3709, "get_random_bytes" },
	{ 0x61a2f3e6, "param_ops_int" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x52c5c991, "__kmalloc_noprof" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x73ca058d, "class_destroy" },
	{ 0x37a0cba, "kfree" },
	{ 0xbf1981cb, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "3021206F5A40D42F5D18B7E");
