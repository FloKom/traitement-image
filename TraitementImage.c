#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>


int* Dynamique(int**data,int* width,int* heigth){
	int* t=calloc(2,sizeof(int));
	t[0]=255;
	t[1]=0;
	for (int i=0;i<*width*(*heigth);i++){
		if(*(*data+i)>t[1]) t[1]=*(*data+i);
		if(*(*data+i)<t[0]) t[0]=*(*data+i);
	}
	return t;	
}

int * readPgmFile(char* chemin,int**data,int* width,int* heigth){
	FILE* ptr;
	char c;
	char pixel[3];
	size_t len=0;
	ssize_t read;
	int tmp=-1;
	int p=0;
	char buf[3];
	int x=0,y=0,i=0,alloc=0,j=0,print=0,width1=0,height=0;
	ptr=fopen(chemin,"r");
	buf[0]=0;
	buf[1]=0;
	buf[2]=0;
	
	if(NULL==ptr){
		printf("file can't be opened /n");
		return NULL;
	}
	c = fgetc(ptr);
    	int fc=1;
    	int h=0;
    	int k=2;// pour ecrire un pixxel en string
    	while (c != EOF)
    	{
	    	if(c=='#'){
	    		fc=1;
	    	}
		if(c == '\n'){
			
			if (fc==0) h++;
			if(fc==1){
				fc=0;
			}
		    //return tmp;
		}
		else{//enteetee
		 if(fc==0 && h>=2){
			if(height*width1>0 && alloc==0){
				alloc=1;
				//printf("======\n");
				*heigth=height;
				*width=width1;
				*data=(int*)calloc(height*width1,sizeof(int));
					
			}else{
				if(c!=' '){
					//printf("%c ",c);
					buf[k]=c;
					k++;
				}else{
					k=0;
					*(*data+x)=atoi(buf);
					//printf("%d ",*(*data+x));
					x++;
					buf[0]=' ';
					buf[1]=' ';
					buf[2]=' ';
					
				}
				
				
			}
			
		}else if (fc==0 && h==0){
				//printf("======\n");
				if(c!=' '){	
					
					//printf("%c",c);
					pixel[y]=c;
					y++;
				}else{
					if(width1==0){ 
						width1=atoi(pixel);
						pixel[0]=' ';
						pixel[1]=' ';
						pixel[2]=' ';
						y=0;
						p=1;	
					}
					//printf("%s\n",pixel);
					//printf("======\n");
					
					
				}	
			}	
		}
		if(p==1 && c=='\n'){ 
				height=atoi(pixel);		//width1=atoi(pixel);
		}else{
		//printf("=====\n");
			//if(height!=atoi(pixel))
				
			
		}
		c = fgetc(ptr);
   	 }
   	 printf("width = %d\n",width1);
	 printf("heigth = %d \n",height);
		
	//printf("%d",i);
	fclose(ptr);
	/*for(int i=0;i<*width;i++){
    		for(int j=0;i<*heigth;j++){
    			printf("%d ",*(*(data+i)+j));
    		}
    		printf("\n");
    	}*/
    	return heigth;
}

void saveImagePGM(char *filename,int **image,int width,int height,int max,char *desc){
    FILE *f = NULL;

    f = fopen(filename, "w");
    if (f == NULL) {
        perror("cannot open file to read");
        exit(EXIT_FAILURE);
    }
    fprintf(f,"P2\n");
    fprintf(f,"#%s\n",desc);
    fprintf(f,"%d %d\n",width,height);
    fprintf(f,"%d\n",max);
    int n;
    for(int i = 0; i<width * height; i++){
        n = *(*image+i);
        if(i % width == 0){
            fprintf(f,"\n");
        }
        if(n<=255){
            fprintf(f,"   %d",n);
        }else if(n > 255){
            fprintf(f,"  255");
        }
    }
    fclose(f);
}


int * Histogramme(int**data,int* width,int* heigth){
	int* hist=calloc(256,sizeof(int));
	for (int i=0;i<*width*(*heigth);i++){
		hist[*(*data+i)]++;
	
	}
	/*for(int i=0;i<256;i++){
		printf("%d == %d \n",i,hist[i]);
	}*/
	return hist;


}


int* transformLinear(int**data,int* width,int* heigth,int max,int min){
	int* res=calloc(*width*(*heigth),sizeof(int));
	int tmp=0,max2=0;
	
	for (int i=0;i<*width*(*heigth);i++){
		
		if(*(*data+i)<=max && *(*data+i)>=min){
		tmp=255*(*(*data+i)-min)/(max-min);
		}else{
		tmp=0;
		}
		if (tmp>255){res[i]=255;}
		else if(tmp<0){res[i]=0;}
		else res[i]=tmp;
		
		if (tmp>max2) max2=tmp;
	}
	saveImagePGM("transform Linear.pgm",&res,*width,*heigth,max2,"Test");
	
	return res;
	

}

int* EgalisationHistogramme(int**data,int* width,int* heigth){
	int* res=calloc(*width*(*heigth),sizeof(int));
	int* hist;
	float tmp=0;int max2=0;
	float T[256],C[256];
	
	hist=Histogramme(data,width,heigth);
	for(int i=0;i<256;i++) {
		T[i]=(float)hist[i]/(*width*(*heigth));
		C[i]=0;
		
		}
	//step 3
	for(int i=0;i<256;i++){
		for(int j=0;j<=i;j++){
			C[i]=C[i]+T[j];
		}
	}
	
	

	for (int i=0;i<*width*(*heigth);i++){
		
		tmp=C[*(*data+i)]*255;
		if (tmp>255){res[i]=255;}
		else if(tmp<0){res[i]=0;}
		else res[i]=(int)tmp;
		
		if (tmp>max2) max2=(int)tmp;
	}
	saveImagePGM("Equalisation.pgm",&res,*width,*heigth,max2,"Test");
	
	return res;
	

}

int* randImg(int width,int height,int max){
	int * T=calloc(width*height,sizeof(int));
	
	for(int i=0;i<width*height;i++){
		T[i]=rand()%(max+1);
	}
	return T;
}

void OR(int**data,int* width,int* heigth,int max){
	int* T=randImg(*width,*heigth,max); //ou avec une image aleatoire
	for(int i=0;i<*width*(*heigth);i++){
		if(T[i]<*(*data+i))
			T[i]=*(*data+i);
	}
	saveImagePGM("Or image.pgm",&T,*width,*heigth,max,"Test");
	free(T);
}

void AND(int**data,int* width,int* heigth,int max){
	int* T=randImg(*width,*heigth,max);
	int max2=0;
	for(int i=0;i<*width*(*heigth);i++){
		if(T[i]>*(*data+i))
			T[i]=*(*data+i);
		if(T[i]>max2) max2=T[i];
	}
	saveImagePGM("And image.pgm",&T,*width,*heigth,max,"Test");
	free(T);
}

void ADD(int**data,int* width,int* heigth,int max){
	int* T=randImg(*width,*heigth,max);
	int max2=0;
	for(int i=0;i<*width*(*heigth);i++){
		T[i]=T[i]+*(*data+i);
		if(T[i]>255) T[i]=255;
		if(T[i]>max2) max2=T[i];
	}
	saveImagePGM("Add image.pgm",&T,*width,*heigth,max,"Test");
	free(T);
}

void SUB(int**data,int* width,int* heigth,int max){
	int* T=randImg(*width,*heigth,max);
	int max2=0;
	for(int i=0;i<*width*(*heigth);i++){
		T[i]=T[i]-*(*data+i);
		if(T[i]<0) T[i]=0;
		if(T[i]>max2) max2=T[i];
	}
	saveImagePGM("Sub image.pgm",&T,*width,*heigth,max,"Test");
	free(T);
}


void MULT(int**data,int* width,int* heigth,int max,int ratio){
	int* T=randImg(*width,*heigth,max);
	int max2=0;
	for(int i=0;i<*width*(*heigth);i++){
		T[i]=(*(*data+i));
		T[i]=T[i]*ratio;	
		if(T[i]>255) T[i]=255;
		if(T[i]>max2) max2=T[i];
	}
	saveImagePGM("Mult image.pgm",&T,*width,*heigth,max,"Test");
	free(T);
}

void INV(int**data,int* width,int* heigth,int max){
	int* T=randImg(*width,*heigth,max);
	int max2=0;
	for(int i=0;i<*width*(*heigth);i++){
		T[i]=255;
		T[i]=T[i]-(*(*data+i));	
		if(T[i]<0) T[i]=0;
		if(T[i]>max2) max2=T[i];
	}
	saveImagePGM("Inversion image.pgm",&T,*width,*heigth,max,"Test");
	free(T);
}

void COMB(int**data,int* width,int* heigth,int max,int ratio1,int ratio2){
	int* T=randImg(*width,*heigth,max);
	int max2=0;
	for(int i=0;i<*width*(*heigth);i++){
		T[i]=(T[i]*ratio1) +(*(*data+i)*ratio2);
		if(T[i]>255) T[i]=255;
		if(T[i]<0) T[i]=0;
		if(T[i]>max2) max2=T[i];
	}
	saveImagePGM("Comb image.pgm",&T,*width,*heigth,max,"Test");
	free(T);
}
int permuter(int i,int j,int width){
	return (i*width)+j;
}

void INTimg(int**data,int* width,int* heigth,int max){
	int* T=randImg(*width*2,*heigth*2,max);
	int tmp=0,tmp2=0;
	for(int i=0;i<*heigth;i++){
		
		for(int j=0;j<*width;j++){
			tmp=permuter(2*i,2*j,*width*2);
			tmp2=permuter(i,j,*width);
			T[tmp]=(*(*data+tmp2));
			tmp=permuter(2*i,(2*j)+1,*width*2);
			T[tmp]=(*(*data+tmp2));
			tmp=permuter((2*i)+1,2*j,*width*2);
			T[tmp]=(*(*data+tmp2));
			tmp=permuter((2*i)+1,(2*j)+1,*width*2);
			T[tmp]=(*(*data+tmp2));
		}
		//printf("+=== i= %d \n",i);
		
		
	}
	saveImagePGM("INT Image.pgm",&T,*width*2,*heigth*2,max,"Test");
	free(T);
}

int* generateFilterMoy(int width){
	int * T=calloc(width*width,sizeof(int));
	
	for(int i=0;i<width*width;i++){
		T[i]=1;
	}
	return T;

}

double* generateFilterGauss(int width,double sigma){
	double * T=calloc(width*width,sizeof(double));
	
	for(int i=0;i<width;i++){
		for(int j=0;i<width;i++){
			T[permuter(i,j,width)]=exp((-1.0)*((i*i)+(j*j))/(2*sigma*sigma));
		}
		
	}
	return T;
}

void Convolution(int**data,int* width,int* heigth,int max,int filter){
	int* T1=generateFilterMoy(filter);
	int* T=randImg(*width,*heigth,max);
	int som=0;
	for(int i=0;i<filter*filter;i++){
		som=som+T1[i];		
	}
	
	int tmp=0,tmp2=0,tmp3=0;
	for(int i=filter/2;i<*heigth-(filter/2);i++){
		
		for(int j=filter/2;j<*width-(filter/2);j++){
			tmp2=permuter(i,j,*width);	
			T[tmp2]=0;
				
			for(int k=0;k<filter;k++){
				
				for(int l=0;l<filter;l++){
					tmp3=permuter(i-(filter/2)+l,j-(filter/2)+k,*width);
					T[tmp2]=T[tmp2]+(*(*data+tmp3)*T1[permuter(k,l,filter)]);	
				
				}
			}
			T[tmp2]=T[tmp2]/som;
			if (T[tmp2]>255) T[tmp2]=255;
			
		}
		//printf("+=== i= %d \n",i);
		
		
	}
	saveImagePGM("Convolution Image.pgm",&T,*width,*heigth,max,"Test");
	free(T);
	free(T1);
}
// void printT(int* T,int taille){
// 	printf("\n ===== TABLE T ======\n");
// 	for(int i=0;i<taille;i++){
// 		printf("%d\t",T[i]);
// 	}
// 	printf("\n ===== END TABLE T ======\n");
	
// }

void Sort(int* T,int taille){
	int a=0,b=0,c=0,flag=0;
	//printT(T,taille);
	for(int i=0;i<taille;i++){
		for(int j=i+1;j<taille;j++){
			if(T[i]>T[j]){
			  a=T[i];
			  T[i]=T[j];
			  T[j]=a;
			}
		}
		
	}
	//printT(T,taille);
}

void medianFilter(int**data,int* width,int* heigth,int max,int filter){
	int* T=randImg(*width,*heigth,max);
	int som=0;
	
	int tmp=0,tmp2=0,tmp3=0;
	
	for(int i=filter/2;i<*heigth-(filter/2);i++){
		
		for(int j=filter/2;j<*width-(filter/2);j++){
			tmp2=permuter(i,j,*width);	
			T[tmp2]=0;
			int* mediane=calloc(filter*filter,sizeof(int));
			int pos=0;
				
			for(int k=0;k<filter;k++){
				
				for(int l=0;l<filter;l++){
					tmp3=permuter(i-(filter/2)+l,j-(filter/2)+k,*width);
					mediane[permuter(k,l,filter)]=(*(*data+tmp3));
				}
			}
			Sort(mediane,filter*filter);
			T[tmp2]=mediane[(filter*filter)/2];
			//printf("+=== Mediane = %d \n",T[tmp2]);			
			//if (T[tmp2]>255) T[tmp2]=255;
			free(mediane);
			break;
			
		}
		//printf("+=== i= %d \n",i);
		
		
	}
	saveImagePGM("Filtre Median image.pgm",&T,*width,*heigth,max,"Test");
	free(T);	
}
int main(int argc, char **argv)
{
  int *data;
  int* hist;
  int* dyn;
  int *transL;
  int width, height;
  if( (argc > 1) && readPgmFile(argv[1],&data, &width, &height) ){
    	printf("BYE\n");
    	hist=Histogramme(&data,&width,&height);
    	dyn=Dynamique(&data,&width,&height);
    	saveImagePGM("test.pgm",&data,width,height,dyn[1],"Test");
    	transL=transformLinear(&data,&width,&height,dyn[1],dyn[0]);
  	free(transL);
  	transL=transformLinear(&data,&width,&height,204,0);
  	free(transL);
  	transL=EgalisationHistogramme(&data,&width,&height);
	free(transL);
  	OR(&data,&width,&height,dyn[1]);
  	AND(&data,&width,&height,dyn[1]);
  	ADD(&data,&width,&height,dyn[1]);
  	SUB(&data,&width,&height,dyn[1]);
  	MULT(&data,&width,&height,dyn[1],1.2);
  	INV(&data,&width,&height,dyn[1]);
  	COMB(&data,&width,&height,dyn[1],1.2,0.5);
  	INTimg(&data,&width,&height,dyn[1]);
 	Convolution(&data,&width,&height,dyn[1], 3);
  	medianFilter(&data,&width,&height,dyn[1], 3);
  	//exp(3.0);
  	free(dyn);
  	free(hist);
  	free(data);
  
  }
    	
  return 0;
}
