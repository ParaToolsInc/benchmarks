! ------------------------
!
      subroutine timestep(time)
      implicit none
      double precision, intent(in) :: time

      integer :: i,j,k

      integer SIZE_OF_MATRIX
      parameter (SIZE_OF_MATRIX = 1000)
      double precision a(SIZE_OF_MATRIX, SIZE_OF_MATRIX)
      double precision b(SIZE_OF_MATRIX, SIZE_OF_MATRIX)
      double precision c(SIZE_OF_MATRIX, SIZE_OF_MATRIX)
      double precision t1a, t1b, t2a, t2b, t3a, t3b

      ! insert matrix multiply tests
      ! initialize matrices
      do i=1,SIZE_OF_MATRIX
        do j=1,SIZE_OF_MATRIX
          a(j,i) = i*time
          b(j,i) = i*time
          c(j,i) = 0D0
        enddo
      enddo

      call cpu_time(t1a)
      do i=1,SIZE_OF_MATRIX
        do j=1,SIZE_OF_MATRIX
          do k=1,SIZE_OF_MATRIX
            c(j,i) = c(j,i)+a(j,i)*b(k,j)
          enddo
        enddo
      enddo
      call cpu_time(t1b)
      !print *, 'naive time: ', t1b-t1a

      c = 0D0

      call cpu_time(t2a)
      do i=1,SIZE_OF_MATRIX
        do k=1,SIZE_OF_MATRIX
          do j=1,SIZE_OF_MATRIX
            c(j,i) = c(j,i)+a(j,i)*b(k,j)
          enddo
        enddo
      enddo
      call cpu_time(t2b)
      !print *, 'better time: ', t2b-t2a

      call cpu_time(t3a)
      c = a * b
      call cpu_time(t3b)
      print *, 'intrinsic time: ', t3b-t3a
      

      return
      end


