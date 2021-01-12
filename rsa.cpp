#include <iostream>
#include <NTL/ZZ.h>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
using namespace NTL;
using namespace std;

class RSA{
private:
    ZZ p;
    ZZ q;
    ZZ d;
    ZZ N;
    ZZ e;
    ZZ o;
    string abc="abcdefghijklmnopqrstuvwxyz ";
public:
    RSA();
    RSA(int n_digi);
    string RSA_encripta();
    string RSA_descifra();
    ZZ aleatorio(int num_digits2);
    ZZ modulo(ZZ a,ZZ b);
    ZZ eucl_ext(ZZ a,ZZ b);
    ZZ mcd(ZZ a,ZZ b);
    ZZ exp(ZZ base,ZZ exponente);
    ZZ exp_mod(ZZ base, ZZ exponente, ZZ mod);
    ZZ resto_chino(ZZ bloque_);
    bool Fermat(ZZ n,ZZ t);
    ZZ generar_primos(int num_digits);
    ZZ get_N(){return N;}
    string ZZto_string(ZZ a);
    ZZ STRto_ZZ(string str);
    string abrir_archivo();
    void guardar_archivo(string mensaje);
    string agregar_0(ZZ num,int K);
    string mensaje_abc(string mensaje,int K);
    string abc_mensaje(string mensaje);
};

/*Valores por defecto*/
RSA::RSA(){
    p=17;
    q=59;
    N=p*q;
    o=(p-1)*(q-1);
    e=3;
}

/*Generar p y q de n digitos*/
RSA::RSA(int n_digi){
    p=generar_primos(n_digi);
    q=generar_primos(n_digi);
    N=p*q;
    o=(p-1)*(q-1);
    e=aleatorio(n_digi/2);
    while(mcd(e,o)!=1){
        e++;
    }
    cout<<"p:"<<p<<"\nq:"<<q<<"\nN:"<<N<<"\no:"<<o<<"\ne:"<<e<<endl;
}

string RSA::RSA_encripta(){
    int K;
    string mensaje,mensaje_;
    cout<<"Palabra a encriptar: "<<endl;
    getline(cin,mensaje);
    K=(ZZto_string(get_N())).size(); //numero de digitos de N
    int i=0;
    string mensaje_cifrado;
    mensaje_=mensaje_abc(mensaje,K);
    while(i<mensaje_.size()){
        string bloque;
        ZZ bloque_,bloque__;
        bloque=mensaje_.substr(i,K-1); //sacamos subcadenas del tamaño de k-1
        bloque_=STRto_ZZ(bloque);
        bloque__=exp_mod(bloque_,e,N);
        mensaje_cifrado=mensaje_cifrado+agregar_0(bloque__,K);
        i=i+K-1;
    }
    guardar_archivo(mensaje_cifrado);
    return mensaje_cifrado;
}

string RSA::RSA_descifra(){
    int K;
    K=(ZZto_string(get_N())).size();
    string mensaje=abrir_archivo();
    d=eucl_ext((p-1)*(q-1),e);
    int i=0;
    string mensaje_descifrado;
    while(i<mensaje.size()){
        string bloque;
        ZZ bloque_,bloque__;
        bloque=mensaje.substr(i,K);
        bloque_=STRto_ZZ(bloque);
        bloque__=exp_mod(bloque_,d,N);
        //bloque__=resto_chino(bloque_);
        mensaje_descifrado=mensaje_descifrado+agregar_0(bloque__,K-1);
        i=i+K;
    }
    return abc_mensaje(mensaje_descifrado);
}

/*Generación de un número aleatorio*/
ZZ RSA::aleatorio(int num_digits2){
    ZZ num_digits;num_digits=num_digits2;
    ZZ flag; flag=1;
    ZZ rep;rep=num_digits/(num_digits/num_digits);
    int i=0;
    ZZ rand1;rand1=RandomBnd(rep+rep) + rep;
    while(i<rep){
        ZZ rand2;rand2=RandomBnd(rand1) + 1;
        flag=flag*rand2;
        i++;
    }
    string num2;num2=ZZto_string(flag);
    num2=num2.substr(0,num_digits2);
    flag=STRto_ZZ(num2);
    return flag;
}

/*Teorema de Fermat, usado para comprobar si un numero
aleatorio es primo o no*/
bool RSA::Fermat(ZZ n,ZZ t){
    ZZ i;
    ZZ a;
    for(i=1;i<t;i++){
        a=RandomBnd(n-1)+2;
        if(exp_mod(a,n-1,n)!=1){
            return false;
        }
    }
    return true;
}
/*Generación de números primos aleatorios*/
ZZ RSA::generar_primos(int num_digits){
    ZZ t;t=2;
    ZZ uno;uno=1;
    /*Se generan numeros aleatorios*/
    ZZ n;n=aleatorio(num_digits);
    n=n+uno;
    /*Se comprueba si dicho numero aleatorio es primo o no.
    Si no lo es, aumentará en una unidad hasta que este
    sea primo*/
    while(Fermat(n,t)==false){
        n=n+uno;
    }
    return n;
}

string RSA::mensaje_abc(string mensaje,int K){
    ZZ num;
    bool o=true;
    string mensaje_;
    for(int j=0;j<mensaje.size();j++){
        num=abc.find(mensaje[j]);
        mensaje_=mensaje_+agregar_0(num,2);
    }
    if(mensaje_.size()>(K-1)){
        while(mensaje_.size()%(K-1)!=0){
        mensaje_=mensaje_+"26";
        }
    }
    else{
        stringstream ss;
        ss<<setfill('0')<<setw(K-1)<<mensaje_;
        mensaje_=ss.str();
        return mensaje_;
    }
    return mensaje_;
}

string RSA::abc_mensaje(string mensaje){
    int num;
    int k=mensaje.size();
    string mensaje_;
    int i=0;
    string bloque;
    while(i<k){
        bloque=mensaje.substr(i,2);
        num=atoi(bloque.c_str());
        mensaje_=mensaje_+abc[num];
        i=i+2;
    }
    return mensaje_;
}

/*Guarda el mensaje encriptado en el archivo "mensaje_cifrado.txt"*/
void RSA::guardar_archivo(string mensaje_){
    ofstream guardar_mensaje;
    string nombre_archivo="mensaje_cifrado.txt";
    guardar_mensaje.open(nombre_archivo.c_str());
    guardar_mensaje<<mensaje_;
    guardar_mensaje.close();
}

/*Abre el archivo "mensaje_cifrado.txt" y retorna su contenido*/
string RSA::abrir_archivo(){
    ifstream archivo;
    string mensaje;
    archivo.open("mensaje_cifrado.txt",ios::in);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
    }
    while(!archivo.eof()){
        getline(archivo,mensaje);
    }
    return mensaje;
}

/*Convierte de entero a string*/
string RSA::ZZto_string(ZZ a){
    stringstream buffer;
    buffer << a;
    return buffer.str();
}

/*Convierte de string a entero*/
ZZ RSA::STRto_ZZ(string str){
    ZZ num(INIT_VAL,str.c_str());
    return num;
}

/*Agrega ceros a la izquierda*/
string RSA::agregar_0(ZZ num,int K){
    stringstream ss;
    string num_=ZZto_string(num);
    ss<<setfill('0')<<setw(K)<<num_;
    return ss.str();
}

/*Reemplaza al operador % (modulo)*/
ZZ RSA::modulo(ZZ a,ZZ b){
    ZZ r;r=a-((a/b)*b);
    if(r<0){
        return r+b;
    }
    else{
        return r;
    }
}

/*MCD*/
ZZ RSA::mcd(ZZ a, ZZ b){
    if (a==0){
        return b;
    }
    else{
        ZZ r;
        while(b>0){
           r=modulo(a,b);
           a=b;
           b=r;
        }
    }
    return a;
}

/*Euclides extendido*/
ZZ RSA::eucl_ext(ZZ a,ZZ b){
    ZZ a_inicial;a_inicial=a;
    ZZ t1,t2,r,q;t1=0,t2=1,r=1,q=0;
    ZZ t;t=0;
    while(r>0){
        q=a/b;
        r=a-q*b;
        a=b;
        b=r;
        t=t2;
        t2=t1-(q*t2);
        t1=t;
    }
    if(t1<0){
        t1=t1+a_inicial;
    }
    return t1;
}

/*Funcion exponencial*/
ZZ RSA::exp(ZZ base,ZZ exponente){
    ZZ resultado;resultado=1;
    while(exponente!=0){
    if(exponente-((exponente>>1)<<1)==1){
        resultado=base*resultado;
    }
    base=base*base;
    exponente=exponente>>1;
    }
    return resultado;

}

/*Exponenciacion modular*/
ZZ RSA::exp_mod(ZZ base,ZZ exponente,ZZ mod){
  ZZ resultado;resultado=1;
  while(exponente!=0){
    base=modulo(base,mod);
    if(exponente-((exponente>>1)<<1)==1){
      resultado=modulo(base*resultado,mod);
    }
    base=base*base;
    exponente=exponente>>1;
  }
  return resultado;
}

/*Teorema del resto chino*/
ZZ RSA::resto_chino(ZZ bloque_){
    d=eucl_ext((p-1)*(q-1),e);
    ZZ dp,dq,p1,p2,q1,q2;
    dp=modulo(d,(p-1));
    dq=modulo(d,(q-1));
    p1=N/p;
    q1=eucl_ext(p1,p);
    p2=N/q;
    q2=eucl_ext(p2,q);
    ZZ a;a=exp(bloque_,dp);
    ZZ b;b=exp(bloque_,dq);
    //cout<<"dp "<<dp<<" dq "<<dq<<" p1 "<<p1<<" q1 "<<q1<<" p2 "<<p2<<" q2 "<<q2<<" a "<<a<<" b "<<b<<endl;
    ZZ Do=modulo(((modulo(a*p1*q1,N))+(modulo(b*p2*q2,N))),N);
    return Do;
}

int main(){
    RSA A;
    A.RSA_encripta();
    cout<<A.RSA_descifra();
    return 0;
}