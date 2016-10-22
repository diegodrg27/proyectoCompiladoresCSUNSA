#include <iostream>
#include <string>
#include <math.h>
#include <map>
#include <utility>

#include <stdio.h>
#include <string.h>
#include <vector>
#include <unordered_map>
#include <fstream>
using namespace std;

#define tamTabla 20
#define caracPolinomial 5
bool comentario = false;

typedef map< string,map<string,string> > tablaTransicion;
tablaTransicion T;

string automata(string identificador){
    string columna;
    string estado = "q0";
    for(int i=0;i<identificador.size();i++){
        if(identificador[i]=='0') columna = "cero";
        else if(identificador[i] >= '1' && identificador[i] <= '7') columna = "numero17";
        else if(identificador[i] >= '8' && identificador[i] <= '9') columna = "numero89";
        else if(identificador[i] == 'o' || identificador[i] == 'O') columna = "caracter";
        else if(identificador[i] == '_') columna = "subguion";
        else if((identificador[i] >= 'a' && identificador[i] <= 'z') || (identificador[i] >= 'A' && identificador[i] <= 'Z')) columna = "letras";
        else {return "error";}

        //estado = T[estado][columna];
        //if(estado=="Error"){cout<<"el numero es invalido"<<endl;return;}
        if(T.find(estado)->second.find(columna)==T.find(estado)->second.end()){return "error";}
        else{estado = T.find(estado)->second.find(columna)->second;}
    }
    if(estado=="q1" || estado=="q4"){return "num";}
    if(estado=="q3"){return "octal";}
    if(estado=="q5"){return "id";}
    else{return "error";}
}

class objectTabla{

	public:
		objectTabla(){
			this -> lexema = "";
			this -> token = "";
			this -> tipoToken = 0;
		}

		objectTabla(string lexema,string token,bool tipoToken){
			this -> lexema = lexema;
			this -> token = token;
			this -> tipoToken = tipoToken;
		}
		string lexema;
		string token;
		bool tipoToken;

		friend ostream& operator << (ostream &o,const objectTabla &p);

	private:
	protected:

};

ostream& operator << (ostream &o,const objectTabla &p)
{
    o << "(" << p.lexema << ", " << p.token << ", "<<p.tipoToken<< ")";
    return o;
}

class Hash{
	private:
		multimap<string,objectTabla> tabla[tamTabla];

		int funcionHash(string clave){
			int valor,total=0;
			for(int i=0;i<clave.length();i++){
				valor = clave[i];
				total+= valor*pow(caracPolinomial,i);
			}
			total = total % tamTabla;
			return total;
		}

	public:
		Hash(){
			//tabla = new objectTabla[tamTabla];
			/*for(int i=0;i<tamTabla;i++){
				tabla[i]=NULL;
			}*/
		}
		virtual ~Hash(){
			//delete []tabla;
		}

		void insertarEspecifico(objectTabla dato){
			string clave = dato.lexema;
			int total = funcionHash(clave);

			tabla[total].insert(pair<string,objectTabla>(clave,dato));

			return;
		}

		string buscar(string clave){
			//bool esta = false;
			int posicion = funcionHash(clave);

			multimap<string,objectTabla>::iterator it=tabla[posicion].find(clave);

			if(it!=tabla[posicion].end()){
				//esta = true;
				return (*it).second.token;
			}

			//cout<<tabla[posicion].find(clave)->second<<endl;
			 /*for (std::multimap<string,objectTabla>::iterator it=tabla[posicion].find(clave); it!=tabla[posicion].end(); ++it)
    			cout << (*it).first << " => " << (*it).second << '\n';*/
			return "vacio";
		}

		//buscarespecifico

		string buscarEspecifico(string clave){
			int posicion = funcionHash(clave);

			multimap<string,objectTabla>::iterator it=tabla[posicion].find(clave);

			if(it!=tabla[posicion].end() && (*it).second.tipoToken==1){
				return (*it).second.token;
			}

			return "vacio";
		}

		void InsertarNoEspecifico(objectTabla dato){ //falta
			string clave = dato.lexema;
			int total = funcionHash(clave);

			tabla[total].insert(pair<string,objectTabla>(clave,dato));

			return;			
		}

		void eliminar(string clave){
			return;
		}
};

Hash tablaSimbolos;
unordered_map<char,string> simbolos( {{';',";"},{'=',"="},{'+',"+"},{'-',"-"},{'*',"*"},{'/',"/"}} );

void generador(string a){
	string palabra;
    vector <string> palabras;
    vector <string> listaTokens;
    int i=0;
    int tam = a.length();

    while(i < tam)
    {
        if(a[i]==' ')
        {
            if (i==0){
                i++;
                continue;
            }
            if(simbolos[a[i]]==""){
                palabras.push_back(palabra);
                palabra.clear();
            }
            else
            {
                i++;
                continue;
            }
        }

        else if(a[i]=='/' && a[i+1]=='/'){
        	if (palabra.size() != 0)
                palabras.push_back(palabra);
            palabra.clear();
        	break;
        }

        /*else if(a[i]=='/' && a[i+1]=='*'){
        	if (palabra.size() != 0)
                palabras.push_back(palabra);
            palabra.clear();
            comentario = true;
        	break;
        }*/

        else if(simbolos[a[i]]!=""){
            if (palabra.size() != 0)
                palabras.push_back(palabra);
            palabras.push_back(simbolos[a[i]]);
            palabra.clear();
        }

        else if(i==a.length()-1)
        {
            palabra=palabra + a[i];
            if(palabra.size() != 0)
                palabras.push_back(palabra);
            break;
        }
        else
            palabra=palabra + a[i];
        i++;
    }

    for(int f=0;f<palabras.size();f++){

        string wordBuffer = tablaSimbolos.buscarEspecifico(palabras[f]);
        if(wordBuffer != "vacio"){
            listaTokens.push_back(wordBuffer);
            cout<<wordBuffer<<" ";
        }

        else{
            string tokenInsert = automata(palabras[f]);
            tablaSimbolos.InsertarNoEspecifico(objectTabla(wordBuffer,tokenInsert,0));
            listaTokens.push_back(tokenInsert);
            cout<<tokenInsert;
            //cout<<"hola";
        }
        cout<<endl;
    }
    

}


int main(int argc, char *argv[]){
	
	/*tabla de transicion*/
        T["q0"]["cero"] = "q1";
        T["q0"]["numero17"] = "q4";
        T["q0"]["numero89"] = "q4";
        T["q0"]["caracter"] = "q5";
        T["q0"]["letras"] = "q5";
        T["q0"]["subguion"] = "q5";

        T["q1"]["cero"] = "q3";
        T["q1"]["numero17"] = "q3";
        T["q1"]["caracter"] = "q2";

        T["q2"]["cero"] = "q3";
        T["q2"]["numero17"] = "q3";

        T["q3"]["cero"] = "q3";
        T["q3"]["numero17"] = "q3";

        T["q4"]["cero"] = "q4";
        T["q4"]["numero17"] = "q4";
        T["q4"]["numero89"] = "q4";

        T["q5"]["cero"] = "q5";
        T["q5"]["numero17"] = "q5";
        T["q5"]["numero89"] = "q5";
        T["q5"]["caracter"] = "q5";
        T["q5"]["letras"] = "q5";
        T["q5"]["subguion"] = "q5";

    /*Tokens Especificos*/

	tablaSimbolos.insertarEspecifico(objectTabla("void","void",1));
	tablaSimbolos.insertarEspecifico(objectTabla("boolean","boolean",1));
	tablaSimbolos.insertarEspecifico(objectTabla("char","char",1));
	tablaSimbolos.insertarEspecifico(objectTabla("byte","byte",1));
	tablaSimbolos.insertarEspecifico(objectTabla("int","int",1));
	tablaSimbolos.insertarEspecifico(objectTabla("word","word",1));
	tablaSimbolos.insertarEspecifico(objectTabla("long","long",1));
	tablaSimbolos.insertarEspecifico(objectTabla("short","short",1));
	tablaSimbolos.insertarEspecifico(objectTabla("float","float",1));
	tablaSimbolos.insertarEspecifico(objectTabla("double","double",1));
	tablaSimbolos.insertarEspecifico(objectTabla("string","string",1));
	tablaSimbolos.insertarEspecifico(objectTabla("error","error",1));
	tablaSimbolos.insertarEspecifico(objectTabla(";","semicolon",1));
	tablaSimbolos.insertarEspecifico(objectTabla("=","igual",1));
	tablaSimbolos.insertarEspecifico(objectTabla("+","plus",1));
	tablaSimbolos.insertarEspecifico(objectTabla("-","menus",1));
	tablaSimbolos.insertarEspecifico(objectTabla("/","division",1));
	tablaSimbolos.insertarEspecifico(objectTabla("%","modulo",1));
	tablaSimbolos.insertarEspecifico(objectTabla("!","not",1));
	tablaSimbolos.insertarEspecifico(objectTabla("<","less",1));
	tablaSimbolos.insertarEspecifico(objectTabla(">","greater",1));
	tablaSimbolos.insertarEspecifico(objectTabla("&","y",1));
	tablaSimbolos.insertarEspecifico(objectTabla("|","o",1));
	tablaSimbolos.insertarEspecifico(objectTabla("#","directv",1));
	tablaSimbolos.insertarEspecifico(objectTabla("{","curbrade",1));
	tablaSimbolos.insertarEspecifico(objectTabla("}","curbraiz",1));

	//cout<<tablaSimbolos.buscar("error")<<endl;
	char line[256];

	generador("int A;//=5;hoasdasbdhsavdbvsadbvasd234567890876543");

}