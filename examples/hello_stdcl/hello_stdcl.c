/* hello_stdcl.c */

#include <stdio.h>
#include <stdcl.h>

int main()
{
   cl_uint n = 64;

	/* use default contexts, if no GPU use CPU */
   CLCONTEXT* cp = (stdgpu)? stdgpu : stdcpu;

   unsigned int devnum = 0;

	clopen(cp,0,CLLD_NOW);

#ifdef __FreeBSD__
   void* clh = clopen(cp,"matvecmult.cl",CLLD_NOW);
   cl_kernel krn = clsym(cp,clh,"matvecmult_kern",0);
#else
   cl_kernel krn = clsym(cp,0,"matvecmult_kern",0);
#endif

   /* allocate OpenCL device-sharable memory */
   cl_float* aa = (float*)clmalloc(cp,n*n*sizeof(cl_float),0);
   cl_float* b = (float*)clmalloc(cp,n*sizeof(cl_float),0);
   cl_float* c = (float*)clmalloc(cp,n*sizeof(cl_float),0);

   /* initialize vectors a[] and b[], zero c[] */
   int i,j; 
   for(i=0;i<n;i++) for(j=0;j<n;j++) aa[i*n+j] = 1.1f*i*j;
   for(i=0;i<n;i++) b[i] = 2.2f*i;
   for(i=0;i<n;i++) c[i] = 0.0f;

   /* define the computational domain and workgroup size */
   clndrange_t ndr = clndrange_init1d( 0, n, 64);

   /* non-blocking sync vectors a and b to device memory (copy to GPU)*/
   clmsync(cp,devnum,aa,CL_MEM_DEVICE|CL_EVENT_NOWAIT);
   clmsync(cp,devnum,b,CL_MEM_DEVICE|CL_EVENT_NOWAIT);

   /* non-blocking fork of the OpenCL kernel to execute on the GPU */
   clforka(cp,devnum,krn,&ndr,CL_EVENT_NOWAIT,n,aa,b,c);

   /* non-blocking sync vector c to host memory (copy back to host) */
   clmsync(cp,devnum,c,CL_MEM_HOST|CL_EVENT_NOWAIT);

   /* force execution of operations in command queue (non-blocking call) */
   clflush(cp,devnum,0);

   /* block on completion of operations in command queue */
   clwait(cp,devnum,CL_ALL_EVENT);

   for(i=0;i<n;i++) printf("%d %f %f\n",i,b[i],c[i]);

   clfree(aa);
   clfree(b);
   clfree(c);

#ifdef __FreeBSD__
	clclose(cp,clh);
#endif

}
