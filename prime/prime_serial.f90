program main

!*****************************************************************************80
!
!! MAIN is the main program for PRIME_MPI.
!
!  Discussion:
!
!    This program calls a version of PRIME_NUMBER that includes
!    MPI calls for parallel processing.
!
!  Licensing:
!
!    This code is distributed under the GNU LGPL license. 
!
!  Modified:
!
!    07 August 2009
!
!  Author:
!
!    John Burkardt
!

  integer ( kind = 4 ) i
  integer ( kind = 4 ) id
  integer ( kind = 4 ) ierr
  integer ( kind = 4 ) n
  integer ( kind = 4 ) n_factor
  integer ( kind = 4 ) n_hi
  integer ( kind = 4 ) n_lo
  integer ( kind = 4 ) p
  integer ( kind = 4 ) primes
  integer ( kind = 4 ) primes_part
  real ( kind = 8 ) wtime
  integer :: a(10000,10000)

  n_lo = 1
!  n_hi = 8192
  n_hi = 65536
  n_factor = 2
  id = 0
  p = 1
!
!
!  Get this process's ID.
!
!  Find out how many processes are available.
!
  if ( id == 0 ) then
    call timestamp ( )
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'PRIME_MPI'
    write ( *, '(a)' ) '  FORTRAN90/MPI version'
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) '  An MPI example program to count the number of primes.'
    write ( *, '(a,i8)' ) '  The number of processes is ', p
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) '         N        Pi          Time'
    write ( *, '(a)' ) ' '
  end if

  n = n_lo

  do while ( n <= n_hi )

    call prime_number ( n, id, p, primes_part )

    write ( *, '(2x,i8,2x,i8)' ) n, primes_part

    n = n * n_factor

  end do 

  do i=1,10000
    do j=1,10000
      a(i,j) = 1
    enddo
  enddo
  print *, 'set a'
!
!
    write ( *, '(a)' ) ' '
    write ( *, '(a)' ) 'PRIME_MPI:'
    write ( *, '(a)' ) '  Normal end of execution.'
    write ( *, '(a)' ) ' '

  stop
end
subroutine prime_number ( n, id, p, total )

!*****************************************************************************80
!
!! PRIME_NUMBER returns a part of the number of primes between 1 and N.
!
!  Discussion:
!
!    In order to divide the work up evenly among P processors, processor
!    ID starts at 2+ID and skips by P.
!
!    A naive algorithm is used.
!
!    Mathematica can return the number of primes less than or equal to N
!    by the command PrimePi[N].
!
!                N  PRIME_NUMBER
!
!                1           0
!               10           4
!              100          25
!            1,000         168
!           10,000       1,229
!          100,000       9,592
!        1,000,000      78,498
!       10,000,000     664,579
!      100,000,000   5,761,455
!    1,000,000,000  50,847,534
!
!  Licensing:
!
!    This code is distributed under the GNU LGPL license. 
!
!  Modified:
!
!    21 May 2009
!
!  Author:
!
!    John Burkardt
!
!  Parameters:
!
!    Input, integer ( kind = 4 ) N, the maximum number to check.
!
!    Input, integer ( kind = 4 ) ID, the ID of this process,
!    between 0 and P-1.
!
!    Input, integer ( kind = 4 ) P, the number of processes.
!
!    Output, integer ( kind = 4 ) TOTAL, the number of prime numbers up to N,
!    starting at 2+ID and skipping by P.
!
  implicit none

  integer ( kind = 4 ) i
  integer ( kind = 4 ) id
  integer ( kind = 4 ) j
  integer ( kind = 4 ) n
  integer ( kind = 4 ) p
  integer ( kind = 4 ) prime
  integer ( kind = 4 ) total

  total = 0

  do i = 2+id, n, p

    prime = 1

    do j = 2, i - 1
      if ( mod ( i, j ) == 0 ) then
        prime = 0
        exit
      end if
    end do

    total = total + prime

  end do

  return
end
subroutine timestamp ( )

!*****************************************************************************80
!
!! TIMESTAMP prints the current YMDHMS date as a time stamp.
!
!  Example:
!
!    31 May 2001   9:45:54.872 AM
!
!  Licensing:
!
!    This code is distributed under the GNU LGPL license.
!
!  Modified:
!
!    18 May 2013
!
!  Author:
!
!    John Burkardt
!
!  Parameters:
!
!    None
!
  implicit none

  character ( len = 8 ) ampm
  integer ( kind = 4 ) d
  integer ( kind = 4 ) h
  integer ( kind = 4 ) m
  integer ( kind = 4 ) mm
  character ( len = 9 ), parameter, dimension(12) :: month = (/ &
    'January  ', 'February ', 'March    ', 'April    ', &
    'May      ', 'June     ', 'July     ', 'August   ', &
    'September', 'October  ', 'November ', 'December ' /)
  integer ( kind = 4 ) n
  integer ( kind = 4 ) s
  integer ( kind = 4 ) values(8)
  integer ( kind = 4 ) y

  call date_and_time ( values = values )

  y = values(1)
  m = values(2)
  d = values(3)
  h = values(5)
  n = values(6)
  s = values(7)
  mm = values(8)

  if ( h < 12 ) then
    ampm = 'AM'
  else if ( h == 12 ) then
    if ( n == 0 .and. s == 0 ) then
      ampm = 'Noon'
    else
      ampm = 'PM'
    end if
  else
    h = h - 12
    if ( h < 12 ) then
      ampm = 'PM'
    else if ( h == 12 ) then
      if ( n == 0 .and. s == 0 ) then
        ampm = 'Midnight'
      else
        ampm = 'AM'
      end if
    end if
  end if

  write ( *, '(i2,1x,a,1x,i4,2x,i2,a1,i2.2,a1,i2.2,a1,i3.3,1x,a)' ) &
    d, trim ( month(m) ), y, h, ':', n, ':', s, '.', mm, trim ( ampm )

  return
end
