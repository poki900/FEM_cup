#include<iostream>
#include<cstdlib>
#include<fstream>
#include<iomanip>
#include<math.h>
#include <cmath>
#include"uklad.cpp"

using namespace std;

void uni_show(int n, int m, double ** tab, string nazwa){
	cout << nazwa << endl;
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++){
			cout.width(9);
			 cout << tab[i][j];// <<"	";
		}
		cout <<endl;
	}
	cout << endl;	
}

void uni_show(int n, double * tab, string nazwa){
	cout << nazwa << endl;
	for(int i=0;i<n;i++){
			cout.width(9);
			 cout << tab[i]; //<<"	";
	}
	cout << endl;	
	cout << endl;	
}

//globalne dane wczytane z pliku
struct GlobalData{
	int SimulationTime=-1;
	int SimulationStepTime=-1;
	double Conductivity=-1;
	int Alfa=-1;
	int Tot=-1;
	int InitialTemp=-1;
	int Density=-1;
	int SpecificHeat=-1;
	double Conductivity2=-1;
	int InitialTemp2=-1;
	int Density2=-1;
	int SpecificHeat2=-1;
	int NoWaterNr=-1;
	int Alfa2=-1;
	
	void GDwrite(double z){
		if(this->SimulationTime<0){
			this->SimulationTime = z;
		}else if(this->SimulationStepTime<0){
			this->SimulationStepTime = z;
		}else if(this->Conductivity<0){
			this->Conductivity = z;
		}else if(this->Alfa<0){
			this->Alfa = z;
		}else if(this->Tot<0){
			this->Tot = z;
		}else if(this->InitialTemp<0){
			this->InitialTemp = z;
		}else if(this->Density<0){
			this->Density = z;
		}else if(this->SpecificHeat<0){
			this->SpecificHeat = z;
		}else if(this->Conductivity2<0){
			this->Conductivity2 = z;
		}else if(this->InitialTemp2<0){
			this->InitialTemp2 = z;
		}else if(this->Density2<0){
			this->Density2 = z;
		}else if(this->SpecificHeat2<0){
			this->SpecificHeat2 = z;
		}else if(this->NoWaterNr<0){
			this->NoWaterNr = z;
		}else if(this->Alfa2<0){
			this->Alfa2 = z;
		}else{
			cout << "Ju¿ zapisano wszystkie dane!";
		}
	}
	
	void show(){
		cout << "Global Data" << endl;
		cout <<"SimulationTime:		"<< this->SimulationTime << endl;
		cout << "SimulationStepTime:	"<< this->SimulationStepTime << endl;
		cout << "Conductivity:		" <<this->Conductivity << endl;
		cout << "Alfa:			" << this->Alfa << endl;
		cout << "Tot:			" << this->Tot << endl;
		cout << "InitialTemp:		" << this->InitialTemp << endl;
		cout << "Density:		" << this->Density << endl;
		cout << "SpecificHeat:		" << this->SpecificHeat << endl;
		cout << "Conductivity2:		" <<this->Conductivity2 << endl;
		cout << "InitialTemp2:		" << this->InitialTemp2 << endl;
		cout << "Density2:		" << this->Density2 << endl;
		cout << "SpecificHeat2:		" << this->SpecificHeat2 << endl ;
		cout << "NoWaterNr:		" << this->NoWaterNr  << endl;
		cout << "Alfa2:			" << this->Alfa2 << endl <<endl;
	}
};

//Uniwersalne wartoœci dla ca³kowania metod¹ Gaussa
struct type{
	int nr;
	double *PC=new double[nr];
	double *W=new double[nr];
	type(int n):nr(n){
		this->read();}
	;
	
	void read(){
		if(nr==2){
			this->PC[0]=sqrt(1.0/3.0)*(-1.0);
			this->PC[1]=sqrt(1.0/3.0);
			this->W[0]=1.0;
			this->W[1]=1.0;
		} else if(nr==3){
			this->PC[0]=sqrt(3.0/5.0)*(-1.0);
			this->PC[1]=0;
			this->PC[2]=sqrt(3.0/5.0);
			this->W[0]=5.0/9.0;
			this->W[1]=8.0/9.0;
			this->W[2]=5.0/9.0;
		} else if(nr==4){
			this->PC[0]=sqrt(3.0/7.0 + 2.0/7.0*sqrt(6.0/5.0))*(-1.0);
			this->PC[1]=sqrt(3.0/7.0 - 2.0/7.0*sqrt(6.0/5.0))*(-1.0);
			this->PC[2]=sqrt(3.0/7.0 - 2.0/7.0*sqrt(6.0/5.0));
			this->PC[3]=sqrt(3.0/7.0 + 2.0/7.0*sqrt(6.0/5.0));
			this->W[0]=(18.0-sqrt(30.0))/36;
			this->W[1]=(18.0+sqrt(30.0))/36;
			this->W[2]=(18.0+sqrt(30.0))/36;
			this->W[3]=(18.0-sqrt(30.0))/36;
		}
	}
};

struct Surface{
	int nr;
	double **N = new double*[nr];
	double *wagi = new double[nr];
	Surface(int n):nr(n){
		for(int i=0; i<nr;i++){
			N[i] = new double[4];
		}
	}
	void wpisz(int i, double ksi, double eta, double wag){
		N[i][0] = 0.25*(1-ksi)*(1-eta);
		N[i][1] = 0.25*(1+ksi)*(1-eta);
		N[i][2] = 0.25*(1+ksi)*(1+eta);
		N[i][3] = 0.25*(1-ksi)*(1+eta);
		wagi[i] = wag;
	}
};

//Element Uniwersalny - element idealnie kwadratowy przeznaczony do przekszta³cenia
struct ElemUni{
	int nr;
	double **ksi=new double*[nr*nr];
	double **eta=new double*[nr*nr];
	double **N=new double*[nr*nr];
	double *wagi = new double[nr*nr];
	Surface *sur[4];
	
	ElemUni(int n):nr(n){
		type tp(nr);
		for(int i=0;i<nr*nr;i++){
			ksi[i]= new double[4];
			eta[i]= new double[4];
			N[i]= new double[4];
			
			eta[i][0]= (-1.0/4.0)*(1.0-tp.PC[i%nr]);
			eta[i][1]= (-1.0/4.0)*(1.0+tp.PC[i%nr]);
			eta[i][2]= (1.0/4.0)*(1.0+tp.PC[i%nr]);
			eta[i][3]= (1.0/4.0)*(1.0-tp.PC[i%nr]);
			
			ksi[i][0]= (-1.0/4.0)*(1.0-tp.PC[i/nr]);
			ksi[i][1]= (1.0/4.0)*(1.0-tp.PC[i/nr]);
			ksi[i][2]= (1.0/4.0)*(1.0+tp.PC[i/nr]);
			ksi[i][3]= (-1.0/4.0)*(1.0+tp.PC[i/nr]);
			
			N[i][0]=1.0/4.0*(1-tp.PC[i%nr])*(1-tp.PC[i/nr]);
			N[i][1]=1.0/4.0*(1+tp.PC[i%nr])*(1-tp.PC[i/nr]);
			N[i][2]=1.0/4.0*(1+tp.PC[i%nr])*(1+tp.PC[i/nr]);
			N[i][3]=1.0/4.0*(1-tp.PC[i%nr])*(1+tp.PC[i/nr]);
			
			wagi[i] = (double)tp.W[i%nr]*tp.W[i/nr];
		}
		
		//uni_show(nr*nr,4,N,"Macierz N");
		
		//obliczanie surface dla kolejnych p³aszczyzn
		for(int i=0;i<4;i++){
			sur[i] = new Surface(nr);
		}
		for(int i=0;i<nr;i++){
			sur[0]->wpisz(i,-1.0,tp.PC[i],tp.W[i]);
		}
		for(int i=0;i<nr;i++){
			sur[1]->wpisz(i,tp.PC[i],-1.0,tp.W[i]);
		}
		for(int i=0;i<nr;i++){
			sur[2]->wpisz(i,1.0,tp.PC[i],tp.W[i]);
		}
		for(int i=0;i<nr;i++){
			sur[3]->wpisz(i,tp.PC[i],1.0,tp.W[i]);
		}
	};
};

//pojedynczy wêze³ wczytany z pliku
struct node{
	double x;
	double y;
	double t;
	bool bc = 0;
};

//pojedynczy element wczytany z pliku
struct element{
	
	int ID[4];
//	double x[4] = {0.0,0.025,0.025,0.0};
//	double y[4] = {0.0,0.0,0.025,0.025};
	double ** H_final = new double*[4];
	double ** C = new double*[4];
	double *P = new double[4];
	
	void jakobian(node *nnd, GlobalData *gd, bool if_first ){
		
		//dane wejœciowe
		int ilu_punktowy = 2;
		ElemUni eu(ilu_punktowy);
		
		//koñcowe wartoœci
		
		double H[ilu_punktowy*ilu_punktowy][4][4];
		for(int i=0;i<4;i++){
			H_final[i] = new double[4];
			C[i] = new double[4];
			for(int j=0;j<4;j++){
				H_final[i][j]=0.0;
				C[i][j]=0.0;
			}
			P[i] = 0.0;
		}
		
		//zmienne dla punktu
		
		double pc[2][2];
		
		double dx[4] = {0.0};
		double dy[4] = {0.0};
		
		double det;
	
		for(int punkt=0;punkt<ilu_punktowy*ilu_punktowy;punkt++){
			
			//zerowanie pc
			for(int i=0;i<2;i++){
				for(int j=0;j<2;j++){
					pc[i][j]=0.0;
				}
			}
			
			for(int i=0;i<4;i++){
//				pc[0][0]+= eu.ksi[punkt][i]*x[i];
//				pc[0][1]+= eu.ksi[punkt][i]*y[i];
//				pc[1][0]+= eu.eta[punkt][i]*x[i];
//				pc[1][1]+= eu.eta[punkt][i]*y[i];
				pc[0][0]+= eu.ksi[punkt][i]*nnd[ID[i]].x;
				pc[0][1]+= eu.ksi[punkt][i]*nnd[ID[i]].y;
				pc[1][0]+= eu.eta[punkt][i]*nnd[ID[i]].x;
				pc[1][1]+= eu.eta[punkt][i]*nnd[ID[i]].y;
			}
//			for(int i=0;i<2;i++){
//				for(int j=0;j<2;j++){
//					cout << pc[i][j] << "	";
//				}
//				cout << endl;
//			}
//				cout << endl;
				
			//macierz odwrotna
			det = pc[0][0]*pc[1][1] - pc[0][1]*pc[1][0]; 
			//cout << det << endl;
			
			for(int i=0;i<2;i++){
				for(int j=0;j<2;j++){
					pc[i][j]=pc[i][j]/det;
					if(i!=j){
						pc[i][j]=-pc[i][j];
					}
				}
			}
			
			double temp = pc[0][0];
			pc[0][0] = pc[1][1];
			pc[1][1] = temp;
			
			for(int i=0;i<4;i++){
				dx[i] = pc[0][0]*eu.ksi[punkt][i] + pc[0][1]*eu.eta[punkt][i]; //dNi/dx
				dy[i] = pc[1][0]*eu.ksi[punkt][i] + pc[1][1]*eu.eta[punkt][i]; //dNi/dy
				//cout << dy[i] << "	";
			}
			//cout << endl;

			//obliczanie macierzy H
			for(int i=0;i<4;i++){
				for(int j=0;j<4;j++){
					if(ID[3]<gd->NoWaterNr){
						H[punkt][i][j] = gd->Conductivity * (dx[i]*dx[j] + dy[i]*dy[j]) * det* eu.wagi[punkt];
						H_final[i][j]+=H[punkt][i][j];
						C[i][j] += (double)gd->SpecificHeat * (double)gd->Density * det * (double)(eu.N[punkt][i]*eu.N[punkt][j]) * (double)eu.wagi[punkt]/gd->SimulationStepTime;
					} else {
						H[punkt][i][j] = gd->Conductivity2 * (dx[i]*dx[j] + dy[i]*dy[j]) * det* eu.wagi[punkt];
						H_final[i][j]+=H[punkt][i][j];
						C[i][j] += (double)gd->SpecificHeat2 * (double)gd->Density2 * det * (double)(eu.N[punkt][i]*eu.N[punkt][j]) * (double)eu.wagi[punkt]/gd->SimulationStepTime;
					}
					//H[punkt][i][j] = gd->Conductivity * (dx[i]*dx[j] + dy[i]*dy[j]) * det* eu.wagi[punkt];
					//H_final[i][j]+=H[punkt][i][j];
					//C[i][j] += (double)gd->SpecificHeat * (double)gd->Density * det * (double)(eu.N[punkt][i]*eu.N[punkt][j]) * (double)eu.wagi[punkt]/gd->SimulationStepTime;
				
				}
			}
			
			
//			cout << "punkt " << punkt << endl;
//			for(int i=0;i<4;i++){
//				for(int j=0;j<4;j++){
//					 cout << H[punkt][i][j] <<"	";
//				}
//				cout <<endl;
//			}
//			cout << endl;
		}
		
		//uni_show(4,4,H_final,"macierz H:");
		
		//uni_show(4,4,C,"macierz C/dt:");
		
		//[HBC]
		
		bool sc[4];
		sc[0] = nnd[ID[0]].bc && nnd[ID[3]].bc;
		sc[1] = nnd[ID[1]].bc && nnd[ID[0]].bc;
		sc[2] = nnd[ID[2]].bc && nnd[ID[1]].bc;
		sc[3] = nnd[ID[3]].bc && nnd[ID[2]].bc;
		
		double bcdet = 0.0;
		double temp = 0.0;
		
		double ** Hbc = new double*[4];
		for(int i=0;i<4;i++){
			Hbc[i] = new double[4];
			for(int j=0;j<4;j++){
				Hbc[i][j] = 0.0;
			}
		}
		
		//if_first = false;
		for(int sciana = 1; sciana<4;sciana+=2){
			if(sc[sciana]){
				
				//d³ugoœæ odcinka w uk³adzie wspó³rzêdnych
				if(sciana>0){
					temp = (nnd[ID[sciana]].x-nnd[ID[sciana-1]].x)*(nnd[ID[sciana]].x-nnd[ID[sciana-1]].x) +(nnd[ID[sciana]].y-nnd[ID[sciana-1]].y)*(nnd[ID[sciana]].y-nnd[ID[sciana-1]].y);
				} else {
					temp = (nnd[ID[0]].x-nnd[ID[3]].x)*(nnd[ID[0]].x-nnd[ID[3]].x) +(nnd[ID[0]].y-nnd[ID[3]].y)*(nnd[ID[0]].y-nnd[ID[3]].y);
				}
				bcdet = sqrt(temp)/2.0;
				
				//obliczanie maczierzy Hbc i P
				for(int pkt=0;pkt<ilu_punktowy;pkt++){
					for(int i=0;i<4;i++){
						for(int j=0;j<4;j++){ 
							//double temp = gd->Alfa * (eu.sur[sciana]->N[pkt][i]*eu.sur[sciana]->N[pkt][j])*eu.sur[sciana]->wagi[pkt]*bcdet;
							double temp =  (eu.sur[sciana]->N[pkt][i]*eu.sur[sciana]->N[pkt][j])*eu.sur[sciana]->wagi[pkt]*bcdet;
							if(if_first){
								if(ID[0]<gd->NoWaterNr/2){
									temp*=gd->Alfa;
									Hbc[i][j]+=temp;
									H_final[i][j]+=temp;
								}else{
									temp*=gd->Alfa2;
									Hbc[3-i][3-j]+=temp;
									H_final[3-i][3-j]+=temp;
								}
							}else {
								if(ID[0]<gd->NoWaterNr/2&&i<2){
									temp*=gd->Alfa;
									Hbc[i][j]+=temp;
									H_final[i][j]+=temp;
								}
							}
						}
						//P[i]+=gd->Alfa *(gd->Tot*eu.sur[sciana]->wagi[pkt]*eu.sur[sciana]->N[pkt][i])*bcdet;
						if(if_first){
							if(ID[0]<gd->NoWaterNr/2){
								if(i<2){
									P[i]-=gd->Alfa *(gd->Tot*eu.sur[sciana]->wagi[pkt]*eu.sur[sciana]->N[pkt][i])*bcdet;
								}
							}
						}else if(ID[0]<gd->NoWaterNr/2&&i<2){
							P[i]-=gd->Alfa *(gd->Tot*eu.sur[sciana]->wagi[pkt]*eu.sur[sciana]->N[pkt][i])*bcdet;
						}
					}
				}
				
			}
		}

		//uni_show(4,4,H_final,"macierz H_final:");
		
		//uni_show(4,4, Hbc,"macierz Hbc:");

		//uni_show(4,P,"macierz P:");	
		
	}
};

struct soe{
	int nN;
	double **H = new double*[nN];
	double **C = new double*[nN]; // [C]/dt
	double **first = new double*[nN]; //([H] + [C]/dt)
	double *P = new double[nN];
	double *temp = new double[nN]; //{ti}
	double *second = new double[nN]; //([C]/dt)*{ti-1} --> ([C]/dt)*{ti-1} + {P}
	soe(int Nodes,GlobalData *gd):nN(Nodes){
		for(int i=0;i<nN;i++){
			H[i] = new double[nN+1];
			C[i] = new double[nN+1];
			first[i] = new double[nN+1];
			for(int j=0;j<nN+1;j++){
				H[i][j] = 0.0;
				C[i][j] = 0.0;
				first[i][j] = 0.0;
			}
			P[i] = 0.0;
			second[i] = 0.0;
			if(i*2<gd->NoWaterNr){
				temp[i] = gd->InitialTemp;
			} else {
				temp[i] = gd->InitialTemp2;
			}
		}
	};
	soe(int Nodes,GlobalData *gd,double *temp2):nN(Nodes){
		for(int i=0;i<nN;i++){
			H[i] = new double[nN+1];
			C[i] = new double[nN+1];
			first[i] = new double[nN+1];
			for(int j=0;j<nN+1;j++){
				H[i][j] = 0.0;
				C[i][j] = 0.0;
				first[i][j] = 0.0;
			}
			P[i] = 0.0;
			second[i] = 0.0;
			temp[i]=temp2[i];
		}
	};
	
	void show(){
		uni_show(nN,nN,H,"Macierz H po agregacji");
		uni_show(nN,nN,C,"Macierz C/dt po agregacji");
		uni_show(nN,P,"Macierz P po agregacji");
	};
	
	void roz( GlobalData *gd){
		second = macierz_wektor(C,temp); //sesond = ([C]/dt)*{ti-1}
		for(int i=0;i<nN;i++){
			second[i]=P[i]+second[i]; //sesond[i] =  ([C]/dt)*{ti-1} + {P} <--wyraz wolny
			for(int j=0;j<nN+1;j++){
				if(j!=nN){
					first[i][j] = H[i][j] + C[i][j]; //([H] + [C]/dt)
				}else{
					first[i][j] = second[i];//([C]/dt)*{ti-1} + {P}
				}
			}
		}
		//uni_show(nN,nN+1,first,"first");
		licz(first,temp); //{ti}
		double suma = 0.0;
		for(int j = gd->NoWaterNr;j<nN;j++){
			suma+=temp[j];
		}
		for(int j = gd->NoWaterNr;j<nN;j++){
			temp[j] = suma/(nN-gd->NoWaterNr);
		}
		//fstream plik("test3.txt", ios::app );
		//plik << suma/(nN-gd->NoWaterNr) << ";" << temp[15] << ";" << temp[8] << endl;
		//plik.close();
		
		uni_show(nN,temp,"");
	}
	double* macierz_wektor(double **macierz, double *wektor){ //mno¿enie macierz * wektor
		double * wynik = new double[nN];
		for(int i=0;i<nN;i++){
			wynik[i] = 0.0;
			for(int j=0;j<nN;j++){
				wynik[i]+= wektor[j]*macierz[i][j];
			}
		}
		return wynik;
	}
	void licz(double **temp, double *wektor){ //rozwi¹zywanie uk³adu równañ --> uk³ad.cpp
		int how_many = nN;
		double ** macierz = new double*[how_many];
		for(int i=0;i<how_many;i++){
			macierz[i] = new double[how_many+1];
			for(int j=0;j<how_many+1;j++){
				macierz[i][j] = temp[i][j];
			}
		}
		bool withoutz = GaussPP(how_many, macierz);
		if (withoutz == 1) {
			//oblicanie x0 .. xn
			//double* tabx = new double[how_many];
			x(how_many, wektor, macierz);
	
			//wyswitlanie x0 .. xn
			//uni_show(nN,wektor,"Wyniki ukladu rownan");
			//delete[] tabx;
		}
	}
};

//siatka wczytana z pliku
struct grid{
	int nN;
	int nE;
	element *Telem = new element[nE];
	node *Tnode = new node[nN];
	grid(int Nodes, int Elements):nN(Nodes),nE(Elements){};
	
	void show_node(){
		cout << "Nodes" <<endl;
		for(int i=0;i<this->nN;i++){
			cout.width(3);
			cout  << i << ":";
			cout.width(11);
			cout << this->Tnode[i].x ;
			cout.width(11);
			cout << this->Tnode[i].y  ;
			cout.width(2);
			cout << this->Tnode[i].bc << endl;
		}
		cout << endl;
	}
	
	void show_elem(){
		cout << "Elements" <<endl;
		for(int i=0;i<this->nE;i++){
			cout << i << ":	";
			for(int j=0;j<4;j++){
				cout << this->Telem[i].ID[j] << "	" ;
			}
			cout << endl;
		}
		cout << endl;
	}
	
	void H_local(GlobalData *gd, bool if_first ){
		for(int el=0; el< nE;el++){
			//cout << endl << "ELEMENT " << el << endl;
			Telem[el].jakobian(Tnode, gd,if_first);
		}
	}
	
	soe agregacja(GlobalData *gd,double *temp){
		soe s(nN,gd, temp);
		
		for(int i=0;i<nN;i++){
			for(int j=0;j<nN;j++){
				s.H[i][j] =0.0;
				s.C[i][j] =0.0;
			}
			s.P[i] =0.0;
		}
		
		for(int el=0;el<nE;el++){
			for(int i=0;i<4;i++){
				for(int j=0;j<4;j++){
					s.H[Telem[el].ID[i]][Telem[el].ID[j]] += Telem[el].H_final[i][j];
					s.C[Telem[el].ID[i]][Telem[el].ID[j]] += Telem[el].C[i][j];
				}
				s.P[Telem[el].ID[i]] += Telem[el].P[i];
			}
		}
		
		//s.show();
		return s;
	}
};

//funkcja odczytuj¹ca dane z pliku - zwraca siatkê grid i przypisuje wartoœci do zmiennej GlobalData
grid odczyt(string name, GlobalData* GD){
	ifstream plik;
    plik.open(name);
    
    int z,n;
    double dz;
    string bin;
    
    for(int i=0; i<8;i++){
    	plik >> bin;
    	plik >> dz;
    	GD->GDwrite(dz);
	}
	//GD->show();
	
	plik >>bin;
	plik >>bin;
	plik >> z;
	plik >>bin;
	plik >>bin;
	plik >>n;
	plik >> bin;
	grid gr(z,n);
	
	for(int i=0;i<gr.nN;i++){
		plik>> bin ;
		plik >> dz;
		gr.Tnode[i].x = dz;
		plik>> bin;
		plik>> dz;
		gr.Tnode[i].y = dz;
		
	}
	
	plik>> bin>>bin>>bin;
	for(int i=0;i<gr.nE;i++){
		for(int j=0;j<4;j++){
			plik>> dz;
			gr.Telem[i].ID[j] = dz-1;
			plik>> bin;
		}
	}
	
	n = -1; //old_z
	plik >>z;
	plik >> bin;
	gr.Tnode[z-1].bc=true;
	do{
		n = z;
		plik >>z;
		plik >> bin;
		gr.Tnode[z-1].bc=true;
	}while(z&&z!=n&&bin!="Conductivity2");
	
	plik >> dz;
    GD->GDwrite(dz);
	
	for(int i=0; i<5;i++){
    	plik >> bin;
    	plik >> z;
    	GD->GDwrite(z);
	}
	
	//gr.show_node();
	//gr.show_elem();
	
    plik.close();
    return gr;
}

void toFile(string name,int i, grid gr, soe s);

int main(){
	//cout << setprecision(10) <<endl;
	GlobalData *gd = new GlobalData();
	//grid gr = odczyt("Test2_4_4_MixGrid.txt", gd);
	//grid gr = odczyt("Test1_4_4.txt", gd);
	//grid gr = odczyt("Test3_31_31_kwadrat.txt", gd);
	//grid gr = odczyt("Test4_31_31_trapez.txt", gd);
	grid gr = odczyt("nazwa3.txt", gd);
	gd->show();
	gr.show_node();
	gr.show_elem();
	
	double * temp = new double[gr.nN];
	for(int i=0;i<gr.nN;i++){
		if(i*2<gd->NoWaterNr){
			temp[i] = gd->InitialTemp;
		} else {
			temp[i] = gd->InitialTemp2;
		}
	}
	bool if_first = true;
	gr.H_local(gd,if_first);
	soe s = gr.agregacja(gd,temp);
	double suma = 0.0;
	for(int i =gd->SimulationStepTime;i<=gd->SimulationTime;i+=gd->SimulationStepTime){
		cout << "krok: " << i;
		s.roz(gd);
		//toFile("group_nazwa_spraw",i,gr,s);
		if(i==gd->SimulationStepTime){
			if_first = false;
			gr.H_local(gd,if_first);
			s = gr.agregacja(gd,s.temp);
		}
	}
	delete gd;
}
void toFile(string name,int n, grid gr, soe s){
	fstream plik( name+"/"+name+to_string(n)+".vtk", ios::app );
	plik << "# vtk DataFile Version 2.0" <<endl;
	plik << "Unstructured Grid Example" <<endl;
	plik << "ASCII" << endl;
	plik << "DATASET UNSTRUCTURED_GRID" << endl << endl;
	
	plik << "POINTS " << gr.nN << " float" << endl;
	for(int j=0;j<gr.nN;j++){
		plik << gr.Tnode[j].x << " " << gr.Tnode[j].y << " 0" << endl;
	}
	plik <<endl;
	
	plik << "CELLS " << gr.nE << " " <<  gr.nE*5 << endl;
	for(int i=0;i<gr.nE;i++){
		plik << "4 ";
		for(int j=0;j<4;j++){
			plik << gr.Telem[i].ID[j] << " " ;
		}
		plik << endl;
	}
	plik << endl;
	
	plik << "CELL_TYPES "<< gr.nE<< endl;
	for(int j=0;j<gr.nE;j++){
		plik << "9" << endl;
	}
	plik <<endl;
	
	plik << "POINT_DATA "<< gr.nN<< endl;
	plik << "SCALARS Temp float 1"<< endl;
	plik << "LOOKUP_TABLE default" << endl;
	for(int j=0;j<gr.nN;j++){
		plik << s.temp[j] << endl;
	}

	plik.close();
}
