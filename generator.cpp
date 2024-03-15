#include<iostream>
#include<cstdlib>
#include<fstream>
#include <cmath>
#include <iostream> 
#include <iomanip>

using namespace std;

#define M_PI 3.14159265358979323846

struct node{
	double x;
	double y;
	double t;
	bool bc = 0;
};

int licz(double nr, double hm, double c, double d,node *Tnode){
	double jed_nr = 2*M_PI/nr;
	double jed_hm = (d-2.0*c)/(2.0*hm);
	//fstream plik( "test.txt", ios::app );
	int p =0;
	//plik << fixed << setprecision(6);
	for(int i=0;i<hm+1;i++){
		for(int j=0;j<nr;j++){
			//(d/2.0-2.0*c)-
			//plik << std::fixed;
			double x,y;
			if(i==0){
				//plik << cos(jed_nr*j)*(d/2) <<";"<< sin(jed_nr*j)*(d/2)<< endl;
				x=cos(jed_nr*j)*(d/2);
				y=sin(jed_nr*j)*(d/2);
			} else{
				//plik << cos(jed_nr*j)*((d/2.0-c)-jed_hm*(i-1)) <<";"<< sin(jed_nr*j)*((d/2.0-c)-jed_hm*(i-1))<< endl;
				x=cos(jed_nr*j)*((d/2.0-c)-jed_hm*(i-1));
				y=sin(jed_nr*j)*((d/2.0-c)-jed_hm*(i-1));
			}
			//cout << (jed_nr*j) << "	" <<cos(jed_nr*j) << endl;
			Tnode[p].x = x;
			Tnode[p].y =y;
			p++;
		}
	}
	Tnode[p].x = 0.0;
	Tnode[p].y =0.0;
	//plik.close();
	return p;
}

int main(){
	cout << fixed << setprecision(6) <<endl;
	cout << "Podaj nazwê pliku: ";
	string name;
	int temp, nr,hm;
	double d,c;
	cin >> name;
	fstream plik( ""+name+".txt", ios::app );
	
	//-------------------------------------DANE WSTÊPNE------------------------------------------
	cout << "Podaj czas symulacji: ";
	cin >> temp;
	plik << "SimulationTime " << temp <<endl;
	
	cout << "Podaj czas kroku symulacji: ";
	cin >> temp;
	plik << "SimulationStepTime " << temp <<endl;
	
	cout << "Conductivity ";
	cin >> d;
	plik << "Conductivity " << d <<endl;
	
	cout << "Alfa ";
	cin >> temp;
	plik << "Alfa " << temp <<endl;
	
	cout << "Tot ";
	cin >> temp;
	plik << "Tot " << temp <<endl;
	
	cout << "InitialTemp ";
	cin >> temp;
	plik << "InitialTemp " << temp <<endl;
	
	cout << "Density ";
	cin >> temp;
	plik << "Density " << temp <<endl;
	
	cout << "SpecificHeat ";
	cin >> temp;
	plik << "SpecificHeat " << temp <<endl;
	
	//-------------------------------------DANE DO WYLICZEÑ------------------------------------------
	cout << "Podaj iloœæ punktów na okrêgu: ";
	cin >> nr;
	cout << "Podaj iloœæ okrêgów: ";
	cin >> hm;
	cout << "Podaj srednice kubka: ";
	cin >> d;
	cout << "Podaj gruboœæ œcianki kubka: ";
	cin >> c;
	
	node *Tnode = new node[nr*(hm+1)+1];
	int p = licz(nr,hm,c,d,Tnode);
	
	plik << "Nodes number " << p+1 << endl;
	plik << "Elements number " << (nr*hm+nr/2) << endl;
	plik << "*Node"<< endl;
	for(int i=0;i<=p;i++){
		plik << fixed << setprecision(6);
		plik << "      " << i+1 << ", " << Tnode[i].x<<", "<< Tnode[i].y << endl;
	}
	
	plik << "*Element, type=DC2D4"<< endl;
	int pom =0;
	int j=1;
	for(int i=0;i<nr*hm+nr/2;i++){
		if(i<nr*hm){
			if(j!=nr){
				plik << " " << i+1 << ", " << i+1 << ", " << i+1+1 << ", "<< i+1+13 << ", "<<  i+1+12<< endl;
			} else{
				plik << " " << i+1 << ", " << i+1 << ", " << i+1+1-nr << ", "<< i+1+13-nr  << ", "<< i+1+12<< endl;
				j=0;
			}
			j++;
			pom = i;
			pom++;
		} else {
			if(j!=nr/2){
				plik << " " << i+1 << ", " << pom+1 << ", " << pom+1+1 << ", "<< pom+2+1 << ", "<< p+1 << endl;
			} else{
				plik << " " << i+1 << ", " << pom+1 << ", " << pom+1+1 << ", "<< pom+2+1-12 << ", "<< p+1 << endl;
				j=0;
			}
			j++;
			pom++;
			pom++;
		}
	}
	
	plik << "*BC"<< endl;
	for(int i=0;i<nr*2;i++){
		plik << i+1;
		if(i+1!=nr*2){
			plik << ", ";
		}
	}
	
	plik << endl << "Conductivity2 0.677" <<endl;
	plik << "InitialTemp2 95" <<endl;
	plik << "Density2 977"  <<endl;
	plik << "SpecificHeat2 4200" <<endl;
	plik << "NoWaterNr " << 2*nr << endl;
	plik << "Alfa2 1912" << endl;
	
	plik.close();
	delete[] Tnode;
}
