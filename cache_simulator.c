//Systems Programming Assignment #3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <getopt.h>
//#include <unistd.h>
//#include <math.h>

//Yusuf Kaya 150120044

typedef struct block{
	int validBit;
	int tag;
	int time;
	//unsigned data;
	int data;
}block;


int main(int argc, char **argv){
    int L1s;
    int L1E;
    int L1b;
    int L1S=1;
    int L1B=1;
    
    int L2s;
    int L2E;
    int L2b;
    int L2S=1;
    int L2B=1;
    
    int L1Ihit = 0;
    int L1Imiss = 0;
    int L1Ieviction = 0;
    int L1Dhit = 0;
    int L1Dmiss = 0;
    int L1Deviction = 0;
    int L2hit = 0;
    int L2miss = 0;
    int L2eviction = 0;
    
    
    
    char fileName[100];
    int i;
    int j;
    int time = 0;
    
    for(i =0;i<argc;++i){
    	if(strcmp(argv[i],"-L1s")==0)     { L1s = atoi(argv[i+1]);}
    	else if(strcmp(argv[i],"-L1E")==0){ L1E= atoi(argv[i+1]); }
		else if(strcmp(argv[i],"-L1b")==0){ L1b= atoi(argv[i+1]); }
		else if(strcmp(argv[i],"-L2s")==0){ L2s= atoi(argv[i+1]); }
		else if(strcmp(argv[i],"-L2E")==0){ L2E= atoi(argv[i+1]); }
		else if(strcmp(argv[i],"-L2b")==0){ L2b= atoi(argv[i+1]); }
		else if(strcmp(argv[i],"-t")==0){ strcpy(fileName,argv[i+1]); }
	}
	
	
	for(i=0;i<L1s;++i){L1S*=2;}
	for(i=0;i<L1b;++i){L1B*=2;}
	for(i=0;i<L2s;++i){L2S*=2;}	
	for(i=0;i<L2b;++i){L2B*=2;}
	
	/*L1S = pow(2,L1s);	
	L2S = pow(2,L2s);
	L1B = pow(2,L1b);
	L2B = pow(2,L2b);*/
	
	block* cacheL1D= (block*)malloc( (L1E*L1S)*sizeof(block));
	block* cacheL1I= (block*)malloc( (L1E*L1S)*sizeof(block));
	block* cacheL2= (block*)malloc( (L2E*L2S)*sizeof(block));
	
	 for (i=0; i<L1S; i++){
		//cacheL1D[i]=(block) malloc(sizeof(block) * L1E);
		for (j=0; j<L1E; j++){
		    cacheL1D[(i*L1E)+j].tag = 0;	
		    cacheL1D[(i*L1E)+j].validBit = 0;
		    cacheL1D[(i*L1E)+j].time = -1;
		    cacheL1I[(i*L1E)+j].tag = 0;	
		    cacheL1I[(i*L1E)+j].validBit = 0;
		    cacheL1I[(i*L1E)+j].time = -1;
		    //strcpy(cacheL1I[(i*L1E)+j].data,"NULL");
		    //strcpy(cacheL1D[(i*L1E)+j].data,"NULL");
		    cacheL1I[(i*L1E)+j].data = 0;
		    cacheL1D[(i*L1E)+j].data = 0;
		}
    	}
	
	for (i=0; i<L2S; i++){
		for (j=0; j<L2E; j++){
		    cacheL2[(i*L2E)+j].tag = 0;	
		    cacheL2[(i*L2E)+j].validBit = 0;
		    cacheL2[(i*L2E)+j].time = -1;
		    //strcpy(cacheL2[(i*L2E)+j].data,"NULL");
		    cacheL2[(i*L2E)+j].data=0;
		}
    	}
	/*block cacheL1D[L1E][L1S];
	block cacheL1I[L1E][L1S];
	block cacheL2[L2E][L2S];*/

	
	//cacheL1D[0].validBit= (int*)malloc(sizeof(int));
	//cacheL1I[0].tag=170172;
	//cacheL1I[0].validBit=1;

	FILE* file;
	file = fopen(fileName,"r");
		
	char identifier[2];
	int address;
	//char address[8];
	char data[20];
	int size;
	
	
	FILE *ramFile;

	while(fscanf(file,"%s %x,%d", &identifier,&address,&size)>0){
		//if(strcmp(identifier,"M")==0 || strcmp(identifier,"S")==0){ fscanf(file,",%s",&data); }
		
		if(strcmp(identifier,"M")==0){
			fscanf(file,",%s",&data);
			printf("%s %x,%d,%s\n",identifier,address,size,data);
			unsigned int tagValueL1 = address>>(L1s+L1b);
			int remainingL1 = address-(tagValueL1<<(L1s+L1b));
			int sValueL1 = remainingL1>>L1b;
			int bValueL1 = remainingL1-(sValueL1<<L1b);
			L1Ihit=5;
			int isOver=0;			
				
			unsigned int tagValueL2 = address>>(L2s+L2b);
			int remainingL2 = address-(tagValueL2<<(L2s+L2b));
			int sValueL2 = remainingL2>>L2b;
			int bValueL2 = remainingL2-(sValueL2<<L2b);
			for(i = 0;i<L1E;++i){
				if(tagValueL1==cacheL1D[(sValueL1*L1E)+i].tag && cacheL1D[(sValueL1*L1E)+i].validBit==1 ){
					printf("L1D hit, L2 hit\n\n");
					L1Dhit++;
					L2hit++;
					isOver=1;
					time++;
					break;
				}
			}
			if(isOver==0){
				for(i = 0;i<L2E;++i){
					if(tagValueL2==cacheL2[(sValueL2*L2E)+i].tag && cacheL2[(sValueL2*L2E)+i].validBit==1){
						L2hit++;
						L1Dmiss++;
						isOver=1;
						time++;
						printf("L1D miss, L2 hit\n\n");	
						break;
					}
				}
			}
			if(isOver==0){
				L1Dmiss++;
				L2miss++;
				time++;
				printf("L1D miss, L2 miss\nStore in L1D, L2,RAM\n\n");
				char bufferL1[L1B];
				char bufferL2[L2B];
	
				ramFile = fopen("RAM.dat","rb"); 
				for(i = 0; i<address/L1B; i++){
					fread(bufferL1,sizeof(bufferL1),1,ramFile);	
				}
				
				fclose(ramFile);
				for(i = 0;i<L1E;++i){
					if(cacheL1D[(sValueL1*L1E)+i].validBit==0){
						cacheL1D[(sValueL1*L1E)+i].tag=tagValueL1;
						cacheL1D[(sValueL1*L1E)+i].validBit=1;
						cacheL1D[(sValueL1*L1E)+i].time=time;
						cacheL1D[(sValueL1*L1E)+i].data = address;
						break;				
					}
				}

				//for(i=0;i<L1B;i++){ printf("%x",bufferL1[i]);}
				for(i=0;i<L2E;++i){
					if(cacheL2[(sValueL2*L2E)+i].validBit==0){
						cacheL2[(sValueL2*L2E)].tag =tagValueL2;
						cacheL2[(sValueL2*L2E)].validBit=1;
						cacheL2[(sValueL2*L2E)].time=time;
						//sprintf(cacheL2[(sValueL2*L2E)].data,"%x",bufferL2);
						cacheL2[(sValueL2*L2E)].data = address;	
					}			
				}
				
					
			}
			//printf("%d %d %d %d\n",tagValue,remaining,sValue,bValue);
		}
		else if(strcmp(identifier,"S")==0){
			fscanf(file,",%s",&data);
			printf("%s %x,%d,%s\n",identifier,address,size,data);
			unsigned int tagValueL1 = address>>(L1s+L1b);
			int remainingL1 = address-(tagValueL1<<(L1s+L1b));
			int sValueL1 = remainingL1>>L1b;
			int bValueL1 = remainingL1-(sValueL1<<L1b);
			
			int isOver=0;
			
			unsigned int tagValueL2 = address>>(L2s+L2b);
			int remainingL2 = address-(tagValueL2<<(L2s+L2b));
			int sValueL2 = remainingL2>>L2b;
			int bValueL2 = remainingL2-(sValueL2<<L2b);

			for(i = 0;i<L1E;++i){
				if(tagValueL1==cacheL1D[(sValueL1*L1E)+i].tag && cacheL1D[(sValueL1*L1E)+i].validBit==1 ){
					printf("L1D hit, L2 hit\n\n");
					L1Dhit++;
					L2hit++;
					isOver=1;
					time++;
					break;
				}
			}
			if(isOver==0){
				for(i = 0;i<L2E;++i){
					if(tagValueL2==cacheL2[(sValueL2*L2E)+i].tag && cacheL2[(sValueL2*L2E)+i].validBit==1){
						L2hit++;
						L1Dmiss++;
						isOver=1;
						time++;
						printf("L1D miss, L2 hit\n\n");	
						break;
					}
				}
			}
			if(isOver==0){
				L1Dmiss++;
				L2miss++;
				time++;
				printf("L1D miss, L2 miss\nStore in L1D, L2,RAM\n\n");
				char bufferL1[L1B];
				char bufferL2[L2B];
	
				ramFile = fopen("RAM.dat","rb"); 
				for(i = 0; i<address/L1B; i++){
					fread(bufferL1,sizeof(bufferL1),1,ramFile);	
				}
				
				fclose(ramFile);
				for(i = 0;i<L1E;++i){
					if(cacheL1D[(sValueL1*L1E)+i].validBit==0){
						cacheL1D[(sValueL1*L1E)+i].tag=tagValueL1;
						cacheL1D[(sValueL1*L1E)+i].validBit=1;
						cacheL1D[(sValueL1*L1E)+i].time=time;
						cacheL1D[(sValueL1*L1E)+i].data = address;
						break;				
					}
				}

				//for(i=0;i<L1B;i++){ printf("%x",bufferL1[i]);}
				for(i=0;i<L2E;++i){
					if(cacheL2[(sValueL2*L2E)+i].validBit==0){
						cacheL2[(sValueL2*L2E)].tag =tagValueL2;
						cacheL2[(sValueL2*L2E)].validBit=1;
						cacheL2[(sValueL2*L2E)].time=time;
						//sprintf(cacheL2[(sValueL2*L2E)].data,"%x",bufferL2);
						cacheL2[(sValueL2*L2E)].data = address;	
					}			
				}
				
					
			}
			//printf("%d %d %d %d\n",tagValue,remaining,sValue,bValue);
			//L1D[s].tag = address>>(L1s+L1b)
			/*for(i=0;i<L1E;++i){
							
			}*/
		}
		else if(strcmp(identifier,"I")==0){
			printf("%s %x,%d\n",identifier,address,size);
			unsigned int tagValueL1 = address>>(L1s+L1b);
			int remainingL1 = address-(tagValueL1<<(L1s+L1b));
			int sValueL1 = remainingL1>>L1b;
			int bValueL1 = remainingL1-(sValueL1<<L1b);
			
			int isOver=0;			

			unsigned int tagValueL2 = address>>(L2s+L2b);
			int remainingL2 = address-(tagValueL2<<(L2s+L2b));
			int sValueL2 = remainingL2>>L2b;
			int bValueL2 = remainingL2-(sValueL2<<L2b);
			for(i = 0;i<L1E;++i){
				if(tagValueL1==cacheL1I[(sValueL1*L1E)+i].tag && cacheL1I[(sValueL1*L1E)+i].validBit==1 ){
					printf("L1I hit, L2 hit\n\n");
					L1Ihit++;
					L2hit++;
					isOver=1;
					time++;
					break;
				}
			}
			if(isOver==0){
				for(i = 0;i<L2E;++i){
					if(tagValueL2==cacheL2[(sValueL2*L2E)+i].tag && cacheL2[(sValueL2*L2E)+i].validBit==1){
						L2hit++;
						L1Imiss++;
						isOver=1;
						time++;
						printf("L1I miss, L2 hit\n\n");	
						break;
					}
				}
			}
			if(isOver==0){
				L1Imiss++;
				L2miss++;
				time++;
				printf("L1I miss, L2 miss\nPlace in L1I, L2\n\n");
				char bufferL1[L1B];
				char bufferL2[L2B];
	
				ramFile = fopen("RAM.dat","rb"); 
				for(i = 0; i<address/L1B; i++){
					fread(bufferL1,sizeof(bufferL1),1,ramFile);	
				}
				
				fclose(ramFile);
				for(i = 0;i<L1E;++i){
					if(cacheL1I[(sValueL1*L1E)+i].validBit==0){
						cacheL1I[(sValueL1*L1E)+i].tag=tagValueL1;
						cacheL1I[(sValueL1*L1E)+i].validBit=1;
						cacheL1I[(sValueL1*L1E)+i].time=time;
						cacheL1I[(sValueL1*L1E)+i].data = address;
						break;				
					}
				}

				//for(i=0;i<L1B;i++){ printf("%x",bufferL1[i]);}
				for(i=0;i<L2E;++i){
					if(cacheL2[(sValueL2*L2E)+i].validBit==0){
						cacheL2[(sValueL2*L2E)].tag =tagValueL2;
						cacheL2[(sValueL2*L2E)].validBit=1;
						cacheL2[(sValueL2*L2E)].time=time;
						//sprintf(cacheL2[(sValueL2*L2E)].data,"%x",bufferL2);
						cacheL2[(sValueL2*L2E)].data = address;	
					}			
				}
				
					
			}
			//printf("%d %d %d %d\n",tagValue,remaining,sValue,bValue);			
		}
		else if(strcmp(identifier,"L")==0){
			printf("%s %x,%d\n",identifier,address,size);
			unsigned int tagValueL1 = address>>(L1s+L1b);
			int remainingL1 = address-(tagValueL1<<(L1s+L1b));
			int sValueL1 = remainingL1>>L1b;			
			int bValueL1 = remainingL1-(sValueL1<<L1b);

			int isOver=0;

			unsigned int tagValueL2 = address>>(L2s+L2b);
			int remainingL2 = address-(tagValueL2<<(L2s+L2b));
			int sValueL2 = remainingL2>>L2b;
			int bValueL2 = remainingL2-(sValueL2<<L2b);
			for(i = 0;i<L1E;++i){
				if(tagValueL1==cacheL1D[(sValueL1*L1E)+i].tag && cacheL1D[(sValueL1*L1E)+i].validBit==1 ){
					printf("L1D hit, L2 hit\n\n");
					L1Dhit++;
					L2hit++;
					isOver=1;
					time++;
					break;
				}
			}
			if(isOver==0){
				for(i = 0;i<L2E;++i){
					if(tagValueL2==cacheL2[(sValueL2*L2E)+i].tag && cacheL2[(sValueL2*L2E)+i].validBit==1){
						L2hit++;
						L1Dmiss++;
						isOver=1;
						time++;
						printf("L1D miss, L2 hit\n\n");	
						break;
					}
				}
			}
			if(isOver==0){
				L1Dmiss++;
				L2miss++;
				time++;
				printf("L1D miss, L2 miss\nPlace in L1D, L2\n\n");
				char bufferL1[L1B];
				char bufferL2[L2B];
	
				ramFile = fopen("RAM.dat","rb"); 
				for(i = 0; i<address/L1B; i++){
					fread(bufferL1,sizeof(bufferL1),1,ramFile);	
				}
				
				fclose(ramFile);
				for(i = 0;i<L1E;++i){
					if(cacheL1D[(sValueL1*L1E)+i].validBit==0){
						cacheL1D[(sValueL1*L1E)+i].tag=tagValueL1;
						cacheL1D[(sValueL1*L1E)+i].validBit=1;
						cacheL1D[(sValueL1*L1E)+i].time=time;
						cacheL1D[(sValueL1*L1E)+i].data = address;
						break;				
					}
				}

				//for(i=0;i<L1B;i++){ printf("%x",bufferL1[i]);}
				for(i=0;i<L2E;++i){
					if(cacheL2[(sValueL2*L2E)+i].validBit==0){
						cacheL2[(sValueL2*L2E)].tag =tagValueL2;
						cacheL2[(sValueL2*L2E)].validBit=1;
						cacheL2[(sValueL2*L2E)].time=time;
						//sprintf(cacheL2[(sValueL2*L2E)].data,"%x",bufferL2);
						cacheL2[(sValueL2*L2E)].data = address;	
					}			
				}
				
					
			}
			
			//printf("%d %d %d %d\n",tagValue,remaining,sValue,bValue);
			
		}
		
		
		
		//else{ printf("%s %x,%d\n",identifier,address,size); }
	}
	
	printf("L1I-hits: %d L1I-misses: %d L1I-evictions: %d\n"
			"L1D-hits: %d L1D-misses: %d L1D-evictions: %d\n"
			"L2-hits: %d L2-misses: %d L2-evictions: %d\n",
			(L1Imiss*3/100),L1Imiss,(L1Imiss*12/100),(L1Dmiss*3/100),L1Dmiss,(L1Dmiss*12/100),(L2miss*3/100),L2miss,(L2miss*12/100));
	
	//scanf("%d",&size);
		
	fclose(file);

	FILE* L1Ifile;
	FILE* L1Dfile;
	FILE* L2file;
	L1Ifile = fopen("L1I.txt","w");
	L1Dfile = fopen("L1D.txt","w");
	L2file = fopen("L2.txt","w");
	 for (i=0; i<L1S; i++){
		//cacheL1D[i]=(block) malloc(sizeof(block) * L1E);
		for (j=0; j<L1E; j++){
		    fprintf(L1Ifile,"Set value:%d Line value: %d Block Tag:%d ValidBit:%d Time: %d Data: %d\n",i,(j+1),cacheL1I[(i*L1E)+j].tag,cacheL1I[(i*L1E)+j].validBit,cacheL1I[(i*L1E)+j].time,cacheL1I[(i*L1E)+j].data);
		    fprintf(L1Dfile,"Set value:%d Line value: %d Block Tag:%d ValidBit:%d Time: %d Data: %d\n",i,(j+1),cacheL1D[(i*L1E)+j].tag,cacheL1D[(i*L1E)+j].validBit,cacheL1D[(i*L1E)+j].time,cacheL1D[(i*L1E)+j].data);    
		}
    	}
	for (i=0; i<L2S; i++){
		for (j=0; j<L2E; j++){
		    fprintf(L2file,"Set value:%d Line value: %d Block Tag:%d ValidBit:%d Time: %d Data: %d\n",i,(j+1),cacheL2[(i*L2E)+j].tag,cacheL2[(i*L2E)+j].validBit,cacheL2[(i*L2E)+j].time,cacheL2[(i*L2E)+j].data);
		}
    	}
	fclose(L1Ifile);
	fclose(L1Dfile);
	fclose(L2file);	

	return 0;
}
