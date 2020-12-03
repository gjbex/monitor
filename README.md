# monitor

Estimating the amount of memory an application will use during execution is
often non trivial, especially when one uses third-party software. However,
this information is valuable, since it helps to determine the characteristics
of the compute nodes a job using this application should run on.

Although the tool presented here can also be used to support the software
development process, better tools are almost certainly available.

Note that currently only single node jobs are supported, MPI support may be
added in a future release.


## Prerequisites

The user is expected to be be familiar with the linux bash shell.


## Monitoring a program

Starting a program, e.g., simulation, to monitor is very straightforward

```bash
$ monitor simulation
```

monitor will write the CPU usage and memory consumption of simulation to
standard error.  Values will be displayed every 5 seconds.  This is the rate
at which monitor samples the program's metrics.

The data is obtained using the `ps` command, so the semantics of the measures
monitor reports is the same as for `ps`.  Quoting from the man page:

```
    %cpu      cpu utilization of the process in "##.#" format.  Currently,
              it is the CPU time used divided by the time the process has
              been running (cputime/realtime ratio), expressed as a
              percentage.  It will not add up to 100% unless you are lucky.

    %mem      ratio of the process's resident set size  to the physical
              memory on the machine, expressed as a percentage.

    size      approximate amount of swap space that would be required if the
              process were to dirty all writable pages and then be swapped
              out.  This number is very rough!
```


## log file

Since monitor's output may interfere with that of the program to monitor,
it is often convenient to use a log file.  The latter can be specified as
follows:

```bash
$ monitor -l simulation.log simulation
```

For long running programs, it may be convenient to limit the output to, e.g.,
the last minute of the programs execution.  Since monitor provides metrics every
5 seconds, this implies we want to limit the output to the last 12 values to
cover a minute:

```bash
$ monitor -l simulation.log -n 12 simulation
```

Note that this option is only available when monitor writes its metrics to a log
file, not when standard error is used.


## Modifying the sample resolution

The interval at which monitor will show the metrics can be modified by
specifying delta, the sample rate:

```bash
$ monitor -d 60 simulation
```

monitor will now print the program's metrics every 60 seconds.  Note that the
minimum delta value is 1 second.


## File sizes

Some programs use temporary files, the size of which may also be a useful
metric.  monitor provides an option to display the size of one or more files
in bytes:

```bash
$ monitor -f tmp/simulation.tmp,cache simulation
```

Here, the size of the file simulation.tmp in directory tmp, as well as the size
of the file cache will be monitored. Files can be specified by absolute as well
as relative path, and multiple files are separated by ','.


## Programs with command line options

Many programs, e.g., matlab, take command line options.  To make sure these do
not interfere with those of monitor and vice versa, the program can for instance
be started in the following way:

```bash
$ monitor -delta 60 -- matlab -nojvm -nodisplay computation.m
```

The use of '--' will ensure that monitor does not get confused by matlab's
'-nojvm' and '-nodisplay' options.


## Subprocesses & multicore programs

Some processes spawn one or more subprocesses.  In that case, the metrics shown
by monitor are aggregated over the process and all of its subprocesses
(recursively).  The reported CPU usage is the sum of all these processes, and
can thus exceed 100 %.

Some (well, since this is a HPC cluster, we hope most) programs use more than one
core to perform their computations.  Hence, it should not come as a surprise that
the CPU usage is reported as larger than 100 %.

When programs of this type are running on a computer with n cores, the CPU usage
can go up to n x 100 %.


## Exit codes

monitor will propagate the exit code of the program it is watching.  Suppose the
latter ends normally, then monitor's exit code will be 0.  On the other hand,
when the program terminates abnormally with a non-zero exit code, e.g., 3, then
this will be monitor's exit code as well.

When monitor has  to terminate in an abnormal state, for instance if it can't
create the log file, its exit code will be 65.  If this interferes with an exit
code of the program to be monitored, it can be modified by setting the
environment variable MONITOR_EXIT_ERROR to a more suitable value.


## Monitoring a running process

It is also possible to "attach" monitor to a program or process that is already
running.  One simply determines the relevant process ID using the ps command,
e.g., 18749, and starts monitor:

```bash
$ monitor -p 18749
```

Note that this feature can be (ab)used to monitor specific subprocesses.


## More information

Help is avaible for monitor by issuing:

```bash
$ monitor -h
### usage: monitor [-d <delta>] [-l <logfile>] [-f <files>]
#                  [-h] [-v] <cmd> | -p <pid>
# Monitor can be used to sample resource utilization of a process
# over time.  Monitor can sample a running process if the latter's PID
# is specified using the -p option, or it can start a command with
# parameters passed as arguments.  When one has to specify flags for
# the command to run, '--' can be used to delimit monitor's options, e.g.,
#    monitor -delta 5 -- matlab -nojvm -nodisplay calc.m
# Resources that can be monitored are memory and CPU utilization, as
# well as file sizes.
# The sampling resolution is determined by delta, i.e., monitor samples
# every <delta> seconds.
# -d <delta>   : sampling interval, specified in
#                seconds, or as [[dd:]hh:]mm:ss
# -l <logfile> : file to store sampling information; if omitted,
#                monitor information is printed on stderr
# -n <lines>   : retain only the last <lines> lines in the log file,
#                note that this option only makes sense when combined
#                with -l, and that the log file lines will not be sorted
#                according to time
# -f <files>   : comma-separated list of file names that are monitored
#                for size; if a file doesn't exist at a given time, the
#                entry will be 'N/A'
# -v           : give verbose feedback
# -h           : print this help message and exit
# <cmd>        : actual command to run, followed by whatever
#                parameters needed
# -p <pid>     : process ID to monitor
#
# Exit status: * 65 for any montor related error
#              * exit status of <cmd> otherwise
# Note: if the exit code 65 conflicts with those of the
#       command to run, it can be customized by setting the
#       environment variables 'MONITOR_EXIT_ERROR' to any value
#       between 1 and 255 (0 is not prohibited, but this is probably.
#       not what you want).
```
