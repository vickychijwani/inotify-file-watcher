WATCHER
=======

Watcher is a program which, when given a list of files to watch (usually in the directory where it is executed), monitors them for modifications and runs a list of arbitrary commands on a file when it is modified. Also, watcher output is color-coded, while output from the commands is in normal text.


Installation Instructions:
--------------------------
From the project root, execute the following commands:

```sh
$ cd src/
$ cc watcher.c -o watcher
```

That should give you an executable file called "watcher". Now copy that executable to the directory in which you want to watch files. Next, create two files in your watched directory:

1. watcher.files : A list of files to be watched, each on a new line with no extra whitespace at either end of the line. The file paths given should be absolute (even the beloved ~ should be replaced with its expansion).

2. watcher.commands : A list of commands to be executed when a watched file is modified. If you want to insert the watched file's path in a command, use "%s" instead of it, and watcher will take care of substituting the file path when running the command.


Use Cases:
----------
1. So you're coding some new idea in a frenzy. Frantically making changes to the code, then going back to the shell to compile and run the code. Hmm, the compilation + execution part is a bit too repititive... isn't my computer supposed to help out with the boring stuff? Watcher's your best friend here. You only have to copy the watcher executable and create 2 simple text files to tell it which file to manipulate and with what commands, each time it gets modified. Then simply run watcher, and have fun writing code, as it compiles and runs automagically. Believe me, it'll shoot up productivity significantly. See the next use case for an example of how to write up the required text files.

2. Let's see the use case which actually served as the inspiration for this tool. I have a git repository of "dotfiles" (i.e., configuration files, including my .bashrc, .emacs, and .gitconfig, among others), and want my computer to be able to automatically make a commit and push it to GitHub when I modify any of those dotfiles, so I don't have to worry about pushing my .emacs to GitHub after the umpteenth change. Cool huh? Oh, and, by the way, the repository I'm talking about is located at https://github.com/vickychijwani/dotfiles. So, I compiled and placed the watcher executable into the local copy of my dotfiles repository and wrote up my watcher.files and watcher.commands like so:

watcher.files :-

```
/home/vicky/.emacs
/home/vicky/.gitconfig
/home/vicky/.bashrc
/home/vicky/.zshrc
/home/vicky/.vimrc
/home/vicky/.bash_aliases
```

watcher.commands :-

```
cp %s .
git commit -am "modified %s"
git push origin master || notify-send "Dotfiles Watcher" "git push failed."
```

See the watcher.commands carefully. It first copies the file, makes a commit to update the modified file with a commit message saying the file was modified (the "%s" represents the path of the modified file), and then it pushes to GitHub (or, if the push fails, notifies me using Ubuntu's native notification system, notify-send).


Caveats:
--------
Watcher uses the Linux "inotify" library which monitors *files*, NOT paths. That means that if you are monitoring the file ~/foo/bar and it gets overwritten with another file somehow, the inotify watch associated with the original file goes away, so it's kinda useless in that case. Also, Emacs and Vim use a backup/save strategy which results in them moving the original file to another location (~/foo/bar~, in the case of Emacs), so it doesn't work if you edit a watched file using Emacs/Vim or a similar text editor (If you're using Emacs, you can disable the automatic creation of backup files, like I have. For that, put this in your .emacs:

```elisp
(setq make-backup-files nil)
```

That'll make Emacs work with Watcher. Beware of the down-sides of disabling automatic backup creation, however.).
