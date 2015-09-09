subroutine stassterms (k12, combine, v3cuse, fn1, fn2, f1r, f1i, f2r, f2i, fdr, fdi, fer, fei, h2pi_s, wex, gind)
implicit none

integer, parameter :: numv3c = 20 ! from fbngrids.fh
integer, intent(in) :: k12
logical, intent(in) :: combine ! .true. in this kernel
real(8), dimension(numv3c), intent(in) :: v3cuse
real(8), intent(out) :: fn1, fn2, f1r, f1i, f2r, f2i, fdr, fdi, fer, fei, wex
complex(8), dimension(5), intent(out) :: h2pi_s, gind


fn1 = -0.140933747272520651d-03 
fn2 = 0.140933747272520651d-03 
f1r = 0.302878965079464880d-03 
f1i = -0.108332304978855703d-04 
f2r = 0.153172489368988153d-03 
f2i = 0.789563353074820644d-04 
fdr = 0.381474003360203853d-03 
fdi = 0.192800809221782733d-03 
fer = -0.578470486169464695d-03 
fei = -0.245146973781184608E-03 
wex = 0.000000000000000000E+00

h2pi_s = cmplx(0d0,0d0)
gind   = cmplx(0d0,0d0)

return
end subroutine stassterms


