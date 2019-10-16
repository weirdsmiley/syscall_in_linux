#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Neon");
MODULE_DESCRIPTION("Fetch members of task_struct via pid_t of a process");

static void printmembers(struct task_struct *);
static void saveinfile(struct task_struct *);

/* Wrapper function for printing the 
 * members of task_struct
 */
static void printmembers(struct task_struct *task)
{
	printk(KERN_INFO "Name\t:\t%s\n", task->comm);		// process name
	printk(KERN_INFO "State\t:\t%ld\n", task->state);	// state
}

static void saveinfile(struct task_struct *task)
{
	struct file *file;
	mm_segment_t prev_filesystem = get_fs();
	set_fs(KERNEL_DS);

	int filedesc = sys_open("/home/neon/sh_task_info.info", O_WRONLY | O_CREAT, 0644);
	if(filedesc >= 0){
		// doing here...
		char data[] = "it feels like";
		//sys_write(filedesc, data,  strlen(data));
		file = fget(filedesc);

		if(file){
			vfs_write(file, data, strlen(data), 0);
			fput(file);
		}
		sys_close(filedesc);
	}
	set_fs(prev_filesystem);
}


/* For a pid, it traverses the task_list 
 * and finds the appropriate task_struct,
 * prints members in kernel log and also 
 * in a file.
 */
static int __init sh_task_info_module_init(void)
{

	struct task_struct *task;
	int pid = 3321;
	for_each_process(task){
		if(task->pid == pid || task->pid == 2){
			printmembers(task);
			saveinfile(task);
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
