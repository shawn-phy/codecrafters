[![progress-banner](https://backend.codecrafters.io/progress/docker/0c8c1350-0f47-4657-9f6f-0cd1b5d6f364)](https://app.codecrafters.io/users/shawn-phy?r=2qF)

This is a starting point for C solutions to the
["Build Your Own Docker" Challenge](https://codecrafters.io/challenges/docker).

In this challenge, you'll build a program that can pull an image from
[Docker Hub](https://hub.docker.com/) and execute commands in it. Along the way,
we'll learn about [chroot](https://en.wikipedia.org/wiki/Chroot),
[kernel namespaces](https://en.wikipedia.org/wiki/Linux_namespaces), the
[docker registry API](https://docs.docker.com/registry/spec/api/) and much more.


# Passing the first stage

The entry point for your Docker implementation is `app/main.c`. Study and
uncomment the relevant code, and push your changes to pass the first stage:

```sh
git add .
git commit -m "pass 1st stage" # any msg
git push origin master
```

That's all! 

# Stage 2 & beyond

Note: This section is for stages 2 and beyond.

You'll use linux-specific syscalls in this challenge. so we'll run your code
_inside_ a Docker container.

Please ensure you have [Docker installed](https://docs.docker.com/get-docker/)
locally.

Next, add a [shell alias](https://shapeshed.com/unix-alias/):

```sh
alias mydocker='docker build -t mydocker . && docker run --cap-add="SYS_ADMIN" mydocker'
```

(The `--cap-add="SYS_ADMIN"` flag is required to create
[PID Namespaces](https://man7.org/linux/man-pages/man7/pid_namespaces.7.html))

You can now execute your program like this:

```sh
mydocker run ubuntu:latest /usr/local/bin/docker-explorer echo hey
```

### stuff I  had to read on to accomplish this:
> child process and fork
>
# stage 3: Handle exit codes 
You'll now pipe the program's stdout and stderr to the parent process.

Like the last stage, the tester will run your program like this:
```
mydocker run ubuntu:latest /usr/local/bin/docker-explorer echo hey
```
To test this behaviour locally, you could use the echo + echo_stderr commands that docker-explorer exposes. Run docker-explorer --help to view usage.

### stuff I  had to read on to accomplish this:
- [ ] linux system calls
- [ ] linux library calls
- [ ] pipe[] in c
- [ ] types.h library in c
- [x] wexit status
- [ ] child process and fork 

# stage 4
In the previous stage, we executed a program that existed locally on our machine. This program had write access to the whole filesystem, which means that it could do dangerous things!

In this stage, you'll use chroot to ensure that the program you execute doesn't have access to any files on the host machine. Create an empty temporary directory and chroot into it when executing the command. You'll need to copy the binary being executed too.

Just like the previous stage, the tester will run your program like this:


### stuff i had to  lear to accomplish this 
- [x] chroot
