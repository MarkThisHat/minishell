# PROGRAM PROGRESS

## Mandatory

- [x] Your shell must implement the following builtins:
	- [x] echo with option -n
	- [x] cd with only a relative or absolute path
	- [x] pwd with no options
	- [x] export with no options
	- [x] unset with no options
	- [x] env with no options or arguments
	- [x] exit with no options
- [x] Display a prompt when waiting for a new command.
- [x] Have a working history.
- [x] Search and launch the right executable (based on the PATH variable or using a relative or an absolute path).
- [x] Not use more than one global variable. Think about it. You will have to explain its purpose.
- [x] Not interpret unclosed quotes or special characters which are not required by the subject such as \ (backslash) or ; (semicolon).
- [x] Handle ’ (single quote) which should prevent the shell from interpreting the meta-characters in the quoted sequence.
- [x] Handle " (double quote) which should prevent the shell from interpreting the meta-characters in the quoted sequence except for $ (dollar sign)
- [ ] Handle $? which should expand to the exit status of the most recently executed foreground pipeline.
- [ ] Handle environment variables ($ followed by a sequence of characters) which should expand to their values.
- [ ] Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash.
- [ ] In interactive mode:
	- [ ] ctrl-C displays a new prompt on a new line.
	- [ ] ctrl-D exits the shell.
	- [ ] ctrl-\ does nothing.
- [ ] Implement redirections:
	- [ ] < should redirect input.
	- [ ] > should redirect output.
	- [ ] << should be given a delimiter, then read the input until a line containing the delimiter is seen. However, it doesn’t have to update the history!
	◦ >> should redirect output in append mode.
- [ ] Implement pipes (| character). The output of each command in the pipeline is connected to the input of the next command via a pipe.
- [ ] Errors treatment.

## Bonus

- [ ] && and || with parenthesis for priorities.
- [ ] Wildcards * should work for the current working directory.