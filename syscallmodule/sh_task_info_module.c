#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/unistd.h>
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
static void saveinfile(struct task_struct *, struct file *);
static struct file *openfile(void);
static void closefile(struct file *);

/* Wrapper function for printing the 
 * members of task_struct
 */
static void printmembers(struct task_struct *task){
	printk(KERN_INFO "Name\t:\t%s\n", task->comm);		// process name
	printk(KERN_INFO "State\t:\t%ld\n", task->state);	// state
}

static struct file *openfile(void){
	struct file *file = NULL;
	int err;
	mm_segment_t prev_filesystem;

	prev_filesystem = get_fs();
	set_fs(get_ds());
	file = filp_open("/home/neon/sh_task_info.txt", O_WRONLY, 0644);
	set_fs(prev_filesystem);
	
	if(IS_ERR(file)){
		err = PTR_ERR(file);
		return NULL;
	}
	return file;
}

static void closefile(struct file *filetoclose){
	filp_close(filetoclose, NULL);
}

static void saveinfile(struct task_struct *task, struct file *file){
	mm_segment_t prev_filesystem;
	prev_filesystem = get_fs();
	set_fs(get_ds());

	vfs_write(file, "hello", strlen("hello"), 0);

	set_fs(prev_filesystem);
}

/*
static void saveinfile(struct task_struct *task){
	struct file *file;
	mm_segment_t prev_filesystem = get_fs();
	set_fs(get_ds());

	file = filp_open("/home/neon/sh_task_info.info", O_WRONLY, 0644);
	set_fs(prev_filesystem);

	if(IS_ERR(file)){
		printk(KERN_ALERT "error: cannot open file\n");
	} else {
		mm_segment_t oldfs;
		oldfs = get_fs();
		set_fs(get_ds());

		
		//prev_filesystem = get_fs();
		//set_fs(get_ds());


		char data[] = "feels";
		//file = fget(filedesc);

		// writing...
		int ret = vfs_write(file, data, strlen(data), 0);
		set_fs(oldfs);
	
		//if(file){
		//	vfs_write(file, data, strlen(data), 0);
		//	fput(file);
		//}

		filp_close(file, NULL);
		//sys_close(filedesc);
	}
	//set_fs(prev_filesystem);
}
*/


/* For a pid, it traverses the task_list 
 * and finds the appropriate task_struct,
 * prints members in kernel log and also 
 * in a file.
 */
static int __init sh_task_info_module_init(void){

	struct task_struct *task;
	int pid = 3321;
	for_each_process(task){
		if(task->pid == pid || task->pid == 2){
			printmembers(task);
			//saveinfile(task);
		}
	}
	
	return 0;
}

static void __exit sh_task_info_module_exit(void){
	printk("exit\n");
}

module_init(sh_task_info_module_init);
module_exit(sh_task_info_module_exit);
