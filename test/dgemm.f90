program dgemm_timing
implicit none

integer :: m = 3000, n = 5000, k = 1000, runs = 2
character(len = 256) :: argv
integer :: i, j, run, argc
real(kind=8), allocatable, dimension(:,:) :: A, B, C
real(kind = 8) :: value, s = 0.0

argc = command_argument_count()
if (argc >= 1) then
    call get_command_argument(1, argv)
    read (argv, '(I8)') runs
end if
if (argc >= 1) then
    call get_command_argument(1, argv)
    read (argv, '(I8)') runs
end if
if (argc >= 2) then
    call get_command_argument(2, argv)
    read (argv, '(I8)') m
end if
if (argc >= 3) then
    call get_command_argument(3, argv)
    read (argv, '(I8)') n
end if
if (argc >= 4) then
    call get_command_argument(4, argv)
    read (argv, '(I8)') k
end if

allocate(A(m, k))
allocate(B(k, n))
allocate(C(m, n))

do run = 1, runs
    do i = 1, m
        do j = 1, k
            call random_number(value)
            A(i, j) = value
        end do
    end do
    do i = 1, k
        do j = 1, n
            call random_number(value)
            B(i, j) = value
        end do
    end do
    call dgemm('N', 'N', m, n, k, 1.0, A, m, B, k, 0.0, C, m)
    s = s + sum(C)/(m*n)
end do
print *, s

deallocate(A)
deallocate(B)
deallocate(C)

end program dgemm_timing

