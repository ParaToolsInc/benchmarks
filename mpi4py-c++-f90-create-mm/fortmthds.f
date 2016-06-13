      subroutine buildgrids(time)
      implicit none
      double precision, intent(in) :: time

      double precision :: dummy1,dummy2
      integer :: i,n

      dummy1 = 0.001
      dummy2 = 1.0

      n = 100000
      do i = 1,n
         dummy1 = dummy1 + dummy2
         dummy1 = dummy1 - dummy2
      end do

      return
      end
!
! ------------------------
!
      subroutine regrid(time)
      implicit none
      double precision, intent(in) :: time

      double precision :: dummy1,dummy2
      integer :: i,n

      dummy1 = 0.001
      dummy2 = 1.0

      n = 100000
      do i = 1,n
         dummy1 = dummy1 + dummy2
         dummy1 = dummy1 - dummy2
      end do

      return
      end

!
! ------------------------
!
      subroutine timestep(time,dt,x,len)
      implicit none
      double precision, intent(in) :: time,dt
      integer, intent(in) :: len
      double precision :: x(*)

      double precision :: dummy1,dummy2
      integer :: i,n
      integer :: j,k

      double precision, allocatable :: dummy3(:)

      integer SIZE_OF_MATRIX
      parameter (SIZE_OF_MATRIX = 2000)
      double precision a(SIZE_OF_MATRIX, SIZE_OF_MATRIX)
      double precision b(SIZE_OF_MATRIX, SIZE_OF_MATRIX)
      double precision c(SIZE_OF_MATRIX, SIZE_OF_MATRIX)

      n = 1000

      dummy1 = 0.001
      dummy2 = 1.0

      allocate(dummy3(n))

      do i = 1,n
         dummy1 = dummy1 + dummy2
         dummy1 = dummy1 - dummy2
         dummy3(i) = dummy2 - dummy1
      end do

      ! Write outside the array bounds
      ! x(len+5) = 1


      ! insert matrix multiply tests
      ! initialize matrices
      do i=1,SIZE_OF_MATRIX
        do j=1,SIZE_OF_MATRIX
          a(j,i) = i
          b(j,i) = i
          c(j,i) = 0D0
        enddo
      enddo

      do i=1,SIZE_OF_MATRIX
        do j=1,SIZE_OF_MATRIX
          do k=1,SIZE_OF_MATRIX
            c(j,i) = c(j,i)+a(j,i)*b(k,j)
          enddo
        enddo
      enddo

      do i=1,SIZE_OF_MATRIX
        do j=1,SIZE_OF_MATRIX
          c(j,i) = 0D0
        enddo
      enddo

      do i=1,SIZE_OF_MATRIX
        do k=1,SIZE_OF_MATRIX
          do j=1,SIZE_OF_MATRIX
            c(j,i) = c(j,i)+a(j,i)*b(k,j)
          enddo
        enddo
      enddo

      do i=1,SIZE_OF_MATRIX
        do j=1,SIZE_OF_MATRIX
          c(j,i) = 0D0
        enddo
      enddo

      c = a * b
      

      return
      end


