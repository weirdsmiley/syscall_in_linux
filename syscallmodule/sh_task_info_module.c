#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Neon");
MODULE_DESCRIPTION("Fetch members of task_struct via pid_t of a process");

static int __init sh_task_info_module_init(void){
	struct task_struct *task_list;
	int pid = 4567;
	for_each_process(task_list){
		if(task_list->pid == pid || task_list->pid == 2 || task_list->pid == 1)
			printk(KERN_INFO "%s\n", task_list->comm);
	}
	
	return 0;
}

static void __exit sh_task_info_module_exit(void){
	printk("exit\n");
}

module_init(sh_task_info_module_init);
module_exit(sh_task_info_module_exit);
