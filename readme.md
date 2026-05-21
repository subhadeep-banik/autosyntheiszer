1 To begin with run

> sudo su
> ./reset

It will clear out the library folder, clear out db.yaml file, and reset the header.h file 


2 to register each implementation run

> gcc -o generate gen.c
> gcc -o register register.c
> ./register -c "config_file"

config file must be in each source directory 

and be of the format 

Security_level : 1
Makefile_Path : ./f1
Toplevel_file_PATH : ./f1
Toplevel_file : aes128.c
Function_name : aes128
Object_name : aes128.o

3 this can be automated with a bash script

TODO bash script....

4 to synthesize run 
> ./synthesize -s <sec_level> -f <e-enc/h-hash> -t <tplevel:0/1/2> -e <energy_level 0/1/2> -p

-p generates plot 


5 run 
> gcc -shared ./LIB/*.o -o lib_enc.so
> export LD_LIBRARY_PATH=:<library_path>

to test ... change the test.c file to change synthesize parameters and run 
> gcc test.c -L./LIB/ -lenc -ldl -rdynamic -o test
> ./test  



