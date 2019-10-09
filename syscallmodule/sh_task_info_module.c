#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Neon");
MODULE_DESCRIPTION("Fetch members of task_struct via pid_t of a process");

static void printmembers(struct task_struct *);

/* Wrapper function for printing the 
 * members of task_struct
 */
static void printmembers(struct task_struct *task_list)
{
	printk(KERN_INFO "%s\n", task_list->comm);
}


/* For a pid, it traverses the task_list 
 * and finds the appropriate task_struct,
 * prints members in kernel log and also 
 * in a file.
 */
static int __init sh_task_info_module_init(void)
{

	struct task_struct *task_list;
	int pid = 3321;
	for_each_process(task_list){
		if(task_list->pid == pid){
			printmembers(task_list);
		}
	}
	
	return 0;
}

static void __exit sh_task_info_module_exit(void)
{
	printk("exit\n");
}

module_init(sh_task_info_module_init);
module_exit(sh_task_info_module_exit);
