all: clean julia

julia: julia_par1.c
	mpicc -o julia julia_par1.c

image: julia.raw julia.gif

julia.raw: julia
	mpirun -c 4 julia -p 256 256

julia.gif: vgaPalette.txt julia.raw
	raw2gif -s 256 256 -p vgaPalette.txt julia.raw > julia.gif 

clean:
	rm julia julia.raw julia.gif 

