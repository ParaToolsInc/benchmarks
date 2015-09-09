program Main
implicit none

real(8), parameter :: MHz = 1600d6 ! CPU frequency, consistent with timebase() function

integer, parameter :: npart = 12, ntrs=220, ncsp = 4, ncpp = 4, ns = 2048, ns_all = 4096
integer, parameter :: numv3c = 20, nsp = 2048, nt = 132, nprs = 66, ntrsp = 221, nsg = 512

real(8) h2m, h2mcsb, pi, drh, drm,rhdrh, rmxrm, fr
common /pass/ h2m, h2mcsb, pi, drh(3,nprs), drm(nprs),rhdrh(3,ntrsp), rmxrm(3,ntrsp), fr(6,nprs)

real(8) fcv,fcspv,fcppv,utv,usv,ustv,utnv,utntv,ulsv,ulstv
common /pairs/ fcv(nprs),fcspv(nprs,ncsp),fcppv(nprs,ncpp),utv(nprs),usv(nprs),ustv(nprs),utnv(nprs),utntv(nprs),ulsv(nprs),ulstv(nprs)

real(8) tstate, tauz, tprot, tneut, tdott, tzptz, t12, tnn, tnp, tpp
common /ispin/ tstate(ns_all+(nsp-ns),nt),tauz(nt,nt,npart), tprot(nt,nt,npart), tneut(nt,nt,npart),tdott(nt,nt,nprs)

integer msval, nsflip, nscyc, nsexchg
integer, pointer :: nsexch(:,:), mss(:,:,:), msss(:,:,:)
common /spin/ msval(ns_all+(nsp-ns),npart), nsflip(nsp,npart), nscyc(ns),nsexch, mss, msss

logical(1) :: nsflip_flip, mss_flip, msss_flip 
logical(1), pointer :: nsexch_flip(:,:)      
common /spin_log1/ nsexch_flip, nsflip_flip(npart), mss_flip(nprs)

real(4) :: flip_sign, nsflip_sign, nscyc_sign
real(4), pointer :: nsexch_sign(:,:), mss_sign(:,:), msss_sign(:,:)  
common /spin_r4/ flip_sign(ns_all), nsflip_sign(nsp), nscyc_sign(ns), nsexch_sign, mss_sign, msss_sign

integer :: k12
real(8), dimension(numv3c) :: v3cuse
real(8), dimension(:,:,:), allocatable :: cl, cr
integer, dimension(:,:,:), allocatable, target :: p_mss
real(4), dimension(:,:), allocatable, target :: p_mss_sign

integer mythread, nthreads, omp_get_thread_num, omp_get_num_threads, i, j, info, jp
real(8), dimension( 2,nsp ) :: ctt
real(8) tdt1, tdt2, tdt3, tdt4, a1, a2, a3, a4, a5, a6, a7, a8

complex(8) u1,u2,u3,u4
complex(8) v1,v2,v3,v4
complex(8) w1,w2,w3,w4
complex(8) x1,x2,x3,x4
complex(8) y1,y2,y3,y4

integer(8) t1,t2,timebase

!$omp parallel private(mythread,nthreads)
    mythread = omp_get_thread_num()
    nthreads = omp_get_num_threads()
    if ( mythread .eq. 0 ) write( 0,'(A,I4)' ) 'Total OMP threads: ', nthreads
!$omp end parallel

allocate( cl( 2,nsp,nt ) )
allocate( cr( 2,nsp,nt ) )
allocate( p_mss( 4,nsg,nprs ) )
allocate( p_mss_sign( nsg,nprs ) )

k12 = 27
v3cuse = 0d0
v3cuse(1) = 0.85875d-5
v3cuse(3) = -0.1296d-5

utv = 0.199948067801762538d-01
usv = -0.202543506125612595E-02
utnv = 0.504747099074154639E-03
ustv = 0.869901808071261344E-03
utntv = -0.347709243999109888E-02

do i = 1, nprs
    fr(1:6,i) = (/ 0.334340566849393395, 0.280912553700335299, 0.379005394924367689, 0.665659433150606605, 0.5, 0.5 /)
enddo

!!!!!!!!!!!! put real data !!!!!!!
do i = 1,nt
    do j = 1,nsp
        cr( 1,j,i ) = sin( DBLE( i+j ) )
        cr( 2,j,i ) = sin( DBLE( i-j ) )
    enddo
enddo

tdott = 1.2
mss_flip = .true.

! values from 1 to ns
do j = 1, nsg
    !do i = 1,4
        !p_mss( i,j,: ) = min( 2*j+1, 2048 )
    !enddo    
    p_mss( 1,j,: ) =    j
    p_mss( 2,j,: ) =  512 + j
    p_mss( 3,j,: ) = 1024 + j
    p_mss( 4,j,: ) = 1536 + j
enddo
mss => p_mss

do i = 1, nsg
    p_mss_sign( i,: ) = sin( REAL(i) )
enddo
mss_sign => p_mss_sign


! intialization done
t1 = timebase()

call stassuij( k12, cl, cr, v3cuse ) ! k12(in), cl(out),cr(in), v3cuse(in)

t2 = timebase()
write( 0,'(A,I18,ES18.10)' ) 'Elapsed time, pclks, s: ', t2 - t1, DBLE( t2 - t1 ) / MHz

write( 0,* ) SUM( cl )
write( 0,* ) '-3.59377469438829555'

deallocate( cl )
deallocate( cr )
deallocate( p_mss )
deallocate( p_mss_sign )

end program Main

