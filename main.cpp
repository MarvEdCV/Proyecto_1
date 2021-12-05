#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <cctype>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>

using namespace std;
using std::cout; using std::cin;
using std::endl; using std::string;
using std::vector; using std::istringstream;
using std::stringstream;

//Variables globales
//TODO:ARREGLAR PATH 
string path="/home/eduardo/Escritorio/ArchivosVacas/Proyecto_1/Proyecto_1";

//ESTRUCTURAS GENERALES
struct Particion
{
    char part_status;
    char part_type = 'P'; // P de primaria como default
    char part_fit = 'W'; // W de peor ajuste por default 
    int part_start;
    int part_size;
    char part_name[16];
};
struct MBR
{
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    string disk_fit;
    Particion mbr_partition[4];
};
struct DiscoD{ 
	string path="";
	int size;
	string unit="M"; /// k=kb m=mb megabytes como default
    string fit="FF";//Primer ajuste como default.
}
Disk1;
void CrearDisco(DiscoD op){
cout<<"************************CREANDO DISCO*************************\n"<<endl;
    string s = path + op.path;//url raiz + url de la entrada
    char sc[s.size() + 1];
    strcpy(sc, s.c_str());
    FILE *file=NULL;
    file=fopen(sc,"r"); //r= read = sera verdadero si el disco ya existe
    if(file!=NULL){
        cout<<"Ya existe el disco"<<endl;
        return;//error
    }
    //Tamaño para distintas unidades que se piden
    int tam;

    if(op.unit=="K"){ //kb
      tam =  op.size* 1024;
    }
    else if(op.unit == "M"){ //mb
        tam =  op.size* 1024 * 1024;
    }

    file=fopen(sc,"wb");
    fwrite("\0",1,1,file);
    /*se pone el puntero en el tamaño deseado del archivo y esto automaticamente 
	hace que el archivo tenga ese size*/
    fseek(file,tam,SEEK_SET); // objeto file, size de cuanto se quiere mover, al inicio
    fwrite("\0",1,1,file);
    //////ESCRITURA DEL MBR/////
    //Modificacion del struct MBR
    MBR mbr;
    mbr.mbr_tamano = tam;
    mbr.mbr_disk_signature = rand()%10000;
    mbr.mbr_fecha_creacion = time(0);
    mbr.disk_fit = op.fit;
    for(int i = 0; i < 4; i++){
        mbr.mbr_partition[i].part_status = '0';
        mbr.mbr_partition[i].part_size = 0;
        mbr.mbr_partition[i].part_fit = 'W';
        mbr.mbr_partition[i].part_start = tam;
        strcpy(mbr.mbr_partition[i].part_name,"");
    }
    
    cout<<"Disco nuevo :: "<<op.path<<"\nFecha de creacion: "<<asctime(gmtime(&mbr.mbr_fecha_creacion))<<endl;
    cout<<"Signature: "<<mbr.mbr_disk_signature <<endl;
    cout<<"Tamaño: "<<mbr.mbr_tamano <<" Bytes"<<endl;
    cout<<"Fit: " <<mbr.disk_fit <<endl;
    cout<<"\n-------------DISCO CREADO--------------------\n"<<endl;
    ///escritura del mbr
    fseek(file,0,SEEK_SET);
    fwrite(&mbr,sizeof(MBR),1,file);
    fclose(file);
}

//FUNCIONES Y METODOS A UTILIZAR 
//Funcion para transformar la entrada a letra mayuscula y asi poder hacer una aplicacion case-insensitive
string CastearMayuscula(char cad1[]){
    int i;
    for ( i = 0; i < strlen(cad1); i++ )  {
    cad1[i] = toupper( cad1[i] );
}
    return cad1;
}
/**
 * Funcion que retorna un vector spliteado por el delimitador que se le ingrese a la cadena deseada para separar.
 * */
vector<string> Split(string cadena,string delimitador){
    size_t pos_start = 0, pos_end, delim_len = delimitador.length();
    string token;
    vector<string> res;

    while ((pos_end = cadena.find (delimitador, pos_start)) != string::npos) {
        token = cadena.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(cadena.substr (pos_start));
    return res;
}
/**
 * Funcion que recibe un string y splitea la cadena por espacios retorna un vector con lo spliteado
 * */
vector<string> SplitSpace(string text){
    vector<string> lineSplit{};
    string word;
    stringstream sstream(text);
    while (getline(sstream,word,' ')){
        lineSplit.push_back(word);
    }
    return lineSplit;
}

void mkcarpetas(string entrada){
    vector<string> aux2;
    aux2 = Split(entrada,"/");//Se crea un vector que esta spliteado por el simbolo / para poder crear las carpetas deseadas por si no existen.
    string variable;//String que almacenara las carpetas a crear o creadas
    for(size_t j=1; j<(aux2.size()-1); j++){//Ciclo que se recorre desde 1 hasta el tamanio del path menos 1 ya que la ultima posicion contiene el nombre deldisco  a crear, se empieza de 1 ya que la posicion 0 es posicion vacia que esta antes de el primer simboo /
        variable =variable+aux2[j]+"/";
        char sc[variable.size() + 1];
        strcpy(sc,variable.c_str());//Casteamos el string a char ya que la funcion mkdir recibe una variable de tipo char* con la direccion del directorio a crear
        mkdir(sc, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //creamos la carpeta recursivamene                       
    }
}
void borrardisco(string ruta){//Metodo que recibe como parametro un path a eliminar.
    if(remove(ruta.c_str())==0) // Eliminamos el archivo
    {
        cout<<"El archivo fue eliminado satisfactoriamente\n"<<endl;
    }
    else{
        cout<<"No se pudo eliminar el archivo\n"<<endl;
    }
}


void EjecutarComando(char comando[200]){
    if(comando[0]!='#'){//Validacion por si viene un comentario antes de una instruccion o comando
        for(int i=0;i<=200;i++){
            if(comando[i]=='#'){//validacion por si viene un comentario al final de la linea
                memcpy(comando,comando,i);//Copiamos la linea del comando pero antes del llegar al comentario
            }
        }
        string comandoCasteado;
        comandoCasteado = CastearMayuscula(comando);//Casteamos todo a mayusculas para trabajarlo internamente porque pueden venir mayusculas y minusculas
        int aa = strncmp(comando,"PAUSE",5);
        if(aa==0){
            int pause;
            cout<<"Script pausado!\n"<<"Presione Enter para continuar\n";
            //pausamos
            pause = cin.get();
        }
        vector<string>lineSplit = SplitSpace(comandoCasteado);//Spliteamos por espacios
            if(lineSplit[0]=="MKDISK"){
                bool unit,fit = false;//Variables booleanas que se estableceran por defecto si no se declaran en el comando si son false se pondran en automatico por defecto segun enunciado
                vector<string> auxiliar;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    if(auxiliar[0] == "-PATH"){//Si el comando es el -path entonces entrara a esta condicional
                        mkcarpetas(auxiliar[1]);//Creamos las carpetas con el path ingresado.
                        Disk1.path = auxiliar[1];//Asignamos el path al disco
                    }
                    else if(auxiliar[0]=="-FIT"){//Asignamos el fit al disco
                        Disk1.fit= auxiliar[1];
                        fit = true;
                    }
                    else if(auxiliar[0]=="-UNIT"){//Asignamos la unidad al disco
                        Disk1.unit = auxiliar[1];
                        unit = true;
                    }
                    else if(auxiliar[0] == "-SIZE"){//Asignamos el tamanio al disco
                        Disk1.size = stof(auxiliar[1]);
                    }
                }
                if(fit==false){//Asignacion default
                    Disk1.fit = "FF";
                }
                if(unit==false){//Asignacion default
                    Disk1.unit ="M";
                }
                CrearDisco(Disk1);//Creamos disco
            }
            if(lineSplit[0]=="RMDISK"){
                string dir;
                vector<string> aux;
                aux = Split(lineSplit[1],"~:~");
                if(aux[0] == "-PATH"){
                    dir = path+aux[1];
                    borrardisco(dir); 
                }           
            }
    }
}

int main(int argc, char const *argv[])
{
    cout<<"********************************************"<<endl;
    cout<<"**          HARD DISK SIMULATION          **"<<endl;
    cout<<"**               201905554                **"<<endl;
    cout<<"******************************************** \n"<<endl;
    char Linea_Comando[200];//Comando a leer con un tamanio estimado maximo de 200 caracteres
    //string delimitador="~:~";
    //vector<string> vectorprueba=Split(Linea_Comando,delimitador);
    //for(auto i:vectorprueba)cout<<i<<endl;
    
    while((string)Linea_Comando!="EXIT"){
        cout<<"Command :: ";
        cin.getline(Linea_Comando,200,'\n');
        string tmp=CastearMayuscula(Linea_Comando);//Casteamos a mayuscula toda la linea para evitarnos problemas del case-insensitive.
        vector<string> ls= SplitSpace(tmp);
        if(ls[0]=="EXEC"){
            cout<<"SOY UN SCRIPT"<<endl;
           //TODO: arreglar esto.
        }
        else{
            EjecutarComando(Linea_Comando);
        }

    }    
    return EXIT_SUCCESS;
}