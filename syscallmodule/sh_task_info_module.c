#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Neon");
MODULE_DESCRIPTION("Fetch members of task_struct via pid_t of a process");

static int __init sh_task_info_module_init(int pid){
	struct task_struct *task_list;

	for_each_process(task_list){
		printk(KERN_INFO "%s\n", task_list->comm);
	}

}

static int __exit sh_task_info_module_exit(void){
	printk("exit\n");
}

module_init(sh_task_info_module_init);
module_exit(sh_task_info_module_exit);
