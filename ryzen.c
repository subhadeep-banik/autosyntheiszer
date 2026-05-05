

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <inttypes.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include <sys/syscall.h>
#include <linux/perf_event.h>

#define AMD_MSR_PWR_UNIT 0xC0010299
#define AMD_MSR_CORE_ENERGY 0xC001029A
#define AMD_MSR_PACKAGE_ENERGY 0xC001029B

#define AMD_TIME_UNIT_MASK 0xF0000
#define AMD_ENERGY_UNIT_MASK 0x1F00
#define AMD_POWER_UNIT_MASK 0xF
#define STRING_BUFFER 1024

#define MAX_CPUS	1024
#define MAX_PACKAGES	16

static int total_cores=0,total_packages=0;
static int package_map[MAX_PACKAGES];

static int detect_packages(void) {

	char filename[BUFSIZ];
	FILE *fff;
	int package;
	int i;

	for(i=0;i<MAX_PACKAGES;i++) package_map[i]=-1;

	printf("\t");
	for(i=0;i<MAX_CPUS;i++) {
		sprintf(filename,"/sys/devices/system/cpu/cpu%d/topology/physical_package_id",i);
		fff=fopen(filename,"r");
		if (fff==NULL) break;
		fscanf(fff,"%d",&package);
		printf("%d (%d)",i,package);
		if (i%8==7) printf("\n\t"); else printf(", ");
		fclose(fff);

		if (package_map[package]==-1) {
			total_packages++;
			package_map[package]=i;
		}

	}

	printf("\n");

	total_cores=i;

	printf("\tDetected %d cores in %d packages\n\n",
		total_cores,total_packages);

	return 0;
}

static int open_msr(int core) {

	char msr_filename[BUFSIZ];
	int fd;

	sprintf(msr_filename, "/dev/cpu/%d/msr", core);
	fd = open(msr_filename, O_RDONLY);
	if ( fd < 0 ) {
		if ( errno == ENXIO ) {
			fprintf(stderr, "rdmsr: No CPU %d\n", core);
			exit(2);
		} else if ( errno == EIO ) {
			fprintf(stderr, "rdmsr: CPU %d doesn't support MSRs\n",
					core);
			exit(3);
		} else {
			perror("rdmsr:open");
			fprintf(stderr,"Trying to open %s\n",msr_filename);
			exit(127);
		}
	}

	return fd;
}

static long long read_msr(int fd, unsigned int which) {

	uint64_t data;

	if ( pread(fd, &data, sizeof data, which) != sizeof data ) {
		perror("rdmsr:pread");
		exit(127);
	}

	return (long long)data;
}
long int readreg(int i){
  long int a;char s[1024];
sprintf(s,"rdmsr -p %d 0xC001029A",i);
  FILE *fp=popen(s, "r");fscanf(fp, "%lx", &a);
  fclose(fp);
  return a;
}
static int rapl_msr_amd_core() {
	unsigned int time_unit, energy_unit, power_unit;
	double time_unit_d, energy_unit_d, power_unit_d;
	
	double *core_energy = (double*)malloc(sizeof(double)*total_cores);
	double *core_energy_delta = (double*)malloc(sizeof(double)*total_cores);
    double *ce = (double*)malloc(sizeof(double)*total_cores);
	double *ced = (double*)malloc(sizeof(double)*total_cores);
	double *package = (double*)malloc(sizeof(double)*total_cores);
	double *package_delta = (double*)malloc(sizeof(double)*total_cores);
	
	int *fd = (int*)malloc(sizeof(int)*total_cores);
	
	for (int i = 0; i < total_cores; i++) {
		fd[i] = open_msr(i);
	}
	
	int core_energy_units = read_msr(fd[0], AMD_MSR_PWR_UNIT);
	printf("Core energy units: %x\n",core_energy_units);
	
	time_unit = (core_energy_units & AMD_TIME_UNIT_MASK) >> 16;
	energy_unit = (core_energy_units & AMD_ENERGY_UNIT_MASK) >> 8;
	power_unit = (core_energy_units & AMD_POWER_UNIT_MASK);
	printf("Time_unit:%d, Energy_unit: %d, Power_unit: %d\n", time_unit, energy_unit, power_unit);
	
	time_unit_d = pow(0.5,(double)(time_unit));
	energy_unit_d = pow(0.5,(double)(energy_unit));
	power_unit_d = pow(0.5,(double)(power_unit));
	printf("Time_unit:%g, Energy_unit: %g, Power_unit: %g\n", time_unit_d, energy_unit_d, power_unit_d);
	
	int core_energy_raw,cer;
	int package_raw;
	// Read per core energy values
	for (int i = 0; i < total_cores; i++) {
		core_energy_raw = read_msr(fd[i], AMD_MSR_CORE_ENERGY);
		//package_raw = read_msr(fd[i], AMD_MSR_PACKAGE_ENERGY);
        cer = readreg(i);
		core_energy[i] = core_energy_raw * energy_unit_d;
        ce[i]= cer * energy_unit_d;
		//package[i] = package_raw * energy_unit_d;
	}

	//usleep(100000);
	system("./a.out");
	for (int i = 0; i < total_cores; i++) {
		core_energy_raw = read_msr(fd[i], AMD_MSR_CORE_ENERGY);
		//package_raw = read_msr(fd[i], AMD_MSR_PACKAGE_ENERGY);
        cer = readreg(i);
		core_energy_delta[i] = core_energy_raw * energy_unit_d;
        ced[i]= cer * energy_unit_d;
		//package_delta[i] = package_raw * energy_unit_d;
	}

	double sum = 0,s=0;
	for(int i = 0; i < total_cores; i++) {
		double diff = (core_energy_delta[i] - core_energy[i]) ;
		double d  = (ced[i] - ce[i]) ;
		sum += diff;
        s+=d;
		printf("Core %d, energy used: %gJ %gJ\n", i, diff,d );
	}
	
	printf("Core sum: %gJ %gJ\n", sum,s);
	
	free(core_energy);
	free(core_energy_delta);
	free(package);
	free(package_delta);
	free(fd);
	
	return 0;
}

int main(int argc, char **argv) {
	detect_packages();
	rapl_msr_amd_core();
	
	return 0;
}
