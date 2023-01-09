# mandelbrot set fractal

## *a multi-threaded C implementation*

[TOC]

# INTRODUCTION

This is a multi-threaded implementation of the famous [Mandelbrot set fractal](https://en.wikipedia.org/wiki/Mandelbrot_set). The goals of the project are (1) to give a simple example of a multi-threaded C programme using POSIX `pthreads`, and (2) to show how to implement mathematical software in C using a functional style. The intended audience is enterprise programmers who are experienced in JavaScript, Java, C#, and similar high-level languages who find themselves on unfamiliar grounds, needing to implement system software or scientific software using a low-level language like C.

# ORGANISATION

This project is structured as follows:

```shell
~/mandelbrot/
  LICENSE          # MIT license
  Makefile         # build script
  complex.[ch]     # complex numbers
  main.c           # main()
  mandelbrot.[ch]  # Mandelbrot set fractal
  thread.[ch]      # POSIX threads
```

This programme is written for Unix-like operating systems. I developed it on macOS. To compile and run this programme, type in the following at a Unix command prompt:

```shell
$ cd ~/mandelbrot
$ make clean all
...
$ ./mandelbrot
...
```

The programme first runs the single-threaded (serial) code and generates a [portable grey map](https://en.wikipedia.org/wiki/Netpbm) image file called `mandelbrot-s.pgm`, followed by the multi-threaded (parallel) code and generates an image file called `mandelbrot-p.pgm`. One image is the inverted greyscale version of the other. This is done so that we can visually distinguish one from the other. Mathematically, the two images are identical.

The files `complex.[ch]` implement complex arithmetic. The `Complex` record type holds two `double` values, which are interpreted either as rectangular coordinates of a complex number $c = x + jy$ (because I am an electrical engineer, I use $j$ instead of $i$) or as polar coordinates of the same complex number $c = \lvert m \rvert \angle{𝜃}$ where the phase angle $𝜃$ is measured in radians. The `ComplexKind` tag identifies the form of the complex number.

There is a pair of constructors for each form: `rOfD()` makes a rectangular form complex number from two `double` values; `rOfP()` makes a rectangular form from a polar form; similarly, `pOfD()` and `pOfR()` make a polar form. That is, `rOfP()` and `pOfR()` are dual converters. Complex, dyadic arithmetic operations `cAdd()`, `cSub()`, `cMul()`, and `cDiv()` take two complex numbers and returns one complex number. `cMod()` computes the modulus, or the magnitude, of a complex number: $\lvert c \rvert = \sqrt{x^2 + y^2}$ or $\lvert m \rvert$. `cArg()` computes the argument, or the phase angle, of a complex number: $𝜃 = tan^{-1}(y / x)$. `cConj()` computes the complex conjugate of the complex number $c$, written $c^* = x - jy = \lvert m \rvert \angle{-\theta}$. And `cSqre()` computes $c^2$. This function is used in computing the Mandelbrot set.

The file `thread.[ch]` provide a simplified interface to POSIX threads. `tRun()` function creates a new thread and, on that thread, applies its worker function argument to its data argument on a newly created thread. Typically, the worker function represents a long-running code. In a single-threaded programme, the client of a long-running function would block, awaiting the result. In a multi-threaded programme, the client—without blocking—forks off a new thread and runs the long-running function on the thread. Normally, the client would create many threads which are executed in parallel, one thread per CPU core. The client then waits for all the threads to complete execution. A record is passed into a thread via `tRun()` function's `data` argument, which is loosely typed `void*`. Inside the worker function, this argument is cast into an appropriate record pointer, and the `data` record is manipulated via that pointer, as needed. The result of the worker function is also returned via the `data` record.

# MATHEMATICS

The Mandelbrot set fractal is named after its discoverer, [Benoît Mandelbrot](https://en.wikipedia.org/wiki/Benoit_Mandelbrot), a Polish-born American mathematician, who is known for his work in the fields of fractal geometry, chaos theory, and complex analysis.

A [fractal](https://en.wikipedia.org/wiki/Fractal) is a geometric object that is recursively self similar at any scale. That is, a zoomed in version of a fractal contains similar, but smaller and differently oriented, copies of the original. Remarkably, a fractal exhibits self similarity at all scales. Fractals exist in nature: coastlines, ice crystals, broccoli heads, to name a few. And there are many mathematical fractals: Julia set, von Koch curve, Sierpiński triangle, etc. The Mandelbrot set $M$—that strange, dark shape in the image below—is the most famous of them all. The self similarity of $M$ is self evident.

![Mandelbrot set fractal](./doc/mandelbrot.jpg)

Although the mathematics that underlies the Mandelbrot set is intricate and expansive, the expression for computing the set $M$ is simple: $z \leftarrow z^2 + c$, where $z$ and $c$ are both complex numbers. This simple expression describes are rather complex (pun intended) dynamic, iterative process.

First, we define a square region on the complex $c$-plane, with the top-left corner at $-3.0 + j2.0$ and the bottom-right corner at $+1.0 - j2.0$. Hence, this region is $4.0 \times 4.0$ in size. Next, we select a complex number $c$ from this region, say the top-left corner. Then, we iterate the expression $z \leftarrow z^2 + c$, where $c$ is the value selected and the initial value of $z$ is set to the origin $0.0 + j0.0$ of the $c$-plane. In mathematics, to iterate a function $f$ on an argument $x$ is to compute $f \circ f \circ ... f\ x$. In functional programming, we may write `x |> f |> f |> ... |> f`, where `|>` means pipe-right.

We terminate the process either when we have iterated a fixed number of times or when the result meets a certain criterion. As we iterate, the value of $z$ chaotically hops about the $c$-plane, and eventually reaching one of these states: falls into the origin if $c$ is selected from inside the set $M$; endlessly swirls along the boundary of $M$ of $c$ itself is on the boundary; or escapes to $∞$ if $c$ is outside $M$. The origin and the $\infty$ are referred to as *attractors* in the complex analysis subfield of mathematics. Here, we iterate $100$ times or until $\lvert z \rvert > 10.0$, whichever comes first. The plot below shows the trajectory of $z$ for $c = -0.74543 + j0.11301$, which is in a neighbourhood of $M$ called the [Seahorse Valley](https://upload.wikimedia.org/wikipedia/commons/5/5b/Mandel_zoom_03_seehorse.jpg), for the repeated seahorse shapes that dot all along the boundary of $M$ in this neighbourhood. Those blue dots that indicate the trajectory of $z$ do indeed resemble a pair of seahorses.

![Mandelbrot set fractal](./doc/iterate.jpg)

When the iteration for a specific $c$ terminates, we associate the final iteration index, referred to as the *escape time*, with the location on the $c$-plane corresponding that $c$. Then, we select another $c$ value, say a small distance $𝛿$ away from the value used in the previous iteration, and repeat the process. We do this until the whole $4.0 \times 4.0$ region has been iterated over in this manner, thus obtaining a map of escape times for all values of $c$ within the region. Mapping these escape times from their original range $[0, 100]$ to the new range $[0, 255]$ yields a viewable greyscale Mandelbrot set, as shown above. We use $𝛿 = 0.001$ here, so we get $4.0\ ÷\ 0.001 = 4,000$ pixels per side of the image.

We may think of our Mandelbrot set generator as a zooming microscope: the $4.0 \times 4.0$ region of interest on the $c$-plane is like the objective lens and the $4,000 \times 4,000$ window on the computer display is like the eyepiece. We can zoom out by expanding the region of interest, and zoom in by contracting it. Whatever the zoom level, we still have the same image size. If we zoom out far enough, the entire Mandelbrot set shrinks down to a dot, and the grey rings that wrap the set look more and more circular. That way lies nothing of interest. The joy of studying the Mandelbrot set is to zoom in, deeper and deeper. Using a simple, immediate-mode GUI, like [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear), it is fairly straightforward to add to this programme an image display with a click-to-zoom functionality. And if desired, we could add a colour palette that maps the greyscale values into RGB colour values.

# IMPLEMENTATION

## *POSIX threads*

On my Mac equipped with a 3.2 GHz, 6-core Intel Core i7 CPU, the 1-thread version of the Mandelbrot set generator takes nearly 20 s, and the 100-thread version takes just 3 s to complete. But before you run off to convert your existing project to multi-threaded versions, recognise that multi-threading is hard, extremely hard. In essence, you are opting to implement your application as a mini operating system, and assuming the risks—and the rewards—that accompany such system-level programming.

The reason I chose the Mandelbrot set problem to demonstrate multi-threaded programming is the inherently simple nature of this problem. Upon close inspection, it is plain to see that each iterative process of $z$ values for a chosen $c$ value is independent of all other iterative processes. Indeed, we could fork a thread for each pixel on the image. But just because we could does not mean we should. Doing so would create $4,000 \times 4,000 = 16,000,000$ threads. One programme spawning that many threads amounts to an assault upon any operating system. Most modern operating systems could handle thousands of thread per program, but the exact number depends on the operating system, the programming language, and the thread library implementations.

A sensible approach is to divide up the region of interest into 100 patches and compute those subsets of $M$ using 100 different threads. Since each patch can be computed independently of all other patches, it is not necessary to coordinate the execution of these threads. In high performance computing world, we refer to this type of problem somewhat derisively as "embarrassingly parallel problem". This unusually high degree of parallelism of the Mandelbrot set generator is why the parallel version is much faster than the serial version.

Most real-world multi-threaded programmes, however, are not that simple, nor are they that embarrassing. A typical multi-threaded programme requires the programmer to control the execution schedule of the threads and to regulate the sharing of the data among them by using mutually exclusive (mutex) locks. The need to coordinate the execution and to sequence the data access rob the threads of their independence and their parallelism.

Be that as it may, in our programme, we need only to create a thread, give it a unique patch of the region of interest along with the thread worker function `mandelbrot()`. This function takes a thread-specific argument `void* data`. It reinterprets this argument as `Mandelbrot* m`, and it accesses the thread-specific top-left and bottom-right bounds of the region of interest as `m→tl` and `m→br`. Each computed greyscale value is then stored in `m→i[y][x]`, where is the pixel intensity of the point $(x, y)$ in the final image. `mandelbrot()` uses its `data` argument both for receiving input and for returning output. Alternatively, this function can return the greyscale image as its output.

C is a loosely typed language, so its type system does not support [parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism). The only practicable way to simulate parametric polymorphism is to cast a pointer of some type, say `Mandelbrot*` to a pointer to the `void` type, namely `void*`.

## *complex numbers*

All eighth graders are acquainted with the nature and the algebra of complex numbers, so I shall jump right into the implementation of the complex numbers module.

The convention in C is to implement a module in the `.c` file and expose the interface via the `.h` file. Here, we represent a complex number as a `struct` containing the kind `enum` tag (`R` for rectangular and `P` for polar) and a pair of `double` values. But we depart from traditional C API design. Traditionally, a C module would include constructors that take a pointer to the record being constructed and return an `int` error code. Here, we provide constructors that take data values and return a constructed record, namely `Complex`: `rOfD()`, `rOfP()`, and so on.

Once the `Complex` records have been created, we manipulate them using operators such as `cAdd()`, `cSub()`, etc., which take and return `Complex` records. These operators have no internal state and they produce no side effects. For example, we add two complex numbers by calling `Complex sum = cAdd(c, d)`, where `c` and `d` are records of the `Complex` type, neither is mutated, a newly instantiated `Complex` record is returned, and there is no hidden state in `cAdd()`. This is the functional style: no pointers, no mutations. In multi-threaded programming, immutability goes a long way.

Note that C passes `struct` by value. So, it would be uneconomical to employ the functional style with a large `struct`. In that case, we are obliged to follow the C convention of taking a pointer to the `struct`, populate it, and return an `int` error code. And the manipulator functions for that large `struct` would also have to take a pointer and mutate the data. In multi-threaded programmes, we would have to employ synchronisation mechanisms—mutex, condition variables, or semaphores—to coordinate multiple threads' access to a single data structure.

# CONCLUSION

This project demonstrates the use of POSIX threads and the mutation-free functional style in C. It is purposefully simple, so that the main concepts are unobscured by the infrastructure- and error-handling bits of code.

Multi-threaded programming seems very difficult to those who are used to writing single-threaded programmes. But to those who have studied the Unix kernel in detail, multi-threaded programmes look natural, even if not easy. So, if you are an experienced programmer who is used to writing line-of-business applications, but you wish to explore multi-threaded system software, your starting point should be a good textbook on the Unix kernel, like the one listed below.

## *resources*

- [*pthread Tutorial*](http://lemuria.cis.vtc.edu/~pchapin/TutorialPthread/pthread-Tutorial.pdf), Chapin
- [*Pthreads Library*](https://docs.oracle.com/cd/E37838_01/html/E61057/tlib-1.html), Oracle
- [`pthread.h`](https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html) man page, OpenGroup.org
- [*The Design of the UNIX Operating System*](https://www.amazon.com/Design-UNIX-Operating-System/dp/0132017997/ref=sr_1_1?crid=16UXOL7NX1WBW&keywords=bach+unix&qid=1673295739&sprefix=bach+unix%2Caps%2C69&sr=8-1), Bach
