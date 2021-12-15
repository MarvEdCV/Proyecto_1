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
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <filesystem>
#include <libgen.h>

using namespace std;
using std::cout; using std::cin;
using std::endl; using std::string;
using std::vector; using std::istringstream;
using std::stringstream;
//Metodos globales
void ParticionPrimaria(int,char,string,char,string);
void PaticionExtendida(int,char,string,char,string);
void ParticionLogica(int,char,string,char,string);
bool ExisteParticion(string,string);
void MontarParticion(string,string);

int FindLogic(string , string );
int FindPrimariaYExtendida(string , string );
void MontarParticion(string , string );
int FindLetra(string , string );
void ImprimirParticinesMontadas();

int FindNumero(string ,string );
void DesmontarParticion(string);
void FormatEXT2(int,int,string);
void FormatEXT3(int,int,string);
void ComandoMKFS(string,string,int);
int VerificarLogin(string,string,string,string);
int FindGrupo(string);
int VerificarParametros(string,string,string);
char ObtenerFit(string,string);
void login(string,string,string);
void logout();
void DesmontarParticionSinMensaje(string,int);

void GraficarMBR(string , string , string );
//mmadf
void MKGRP(string);
int ObtenerIDG();
void SaveJournaling(char* ,int ,int ,char *,char *);
void AddUserstxt(string );
int FindBit(FILE *, char , char );
void RMGRP(string);
void REMOVERGRUPO(string);

void MKUSR(string , string , string );
bool buscarUsuario(string );
int getID_usr();
void RMUSR(string );
void eliminarUsuario(string );
void cambiarPermisosRecursivo(FILE* , int , int perisos);
void CHMOD(string , string ,bool );
int buscarCarpetaArchivo(FILE *, char* );
int byteInodoBloque(FILE *,int , char );
void MKFILE(string ,string ,int ,bool ,string );
int crearArchivo(string , bool , int , string );
int nuevoArchivo(FILE *, char , bool , char *, int , string , int ,char *);

bool permisosDeEscritura(int , bool , bool );
string EliminarComillas(string );
int nuevaCarpeta(FILE *, char , bool , char *, int );
string getDirectorio(string );
void MKDIR(string ,string ,bool );
int crearCarpeta(string , bool p);


void GraficarMBR(string , string , string );
void GraficarDISCO(string , string , string );
void REPORTES(string ,string ,string ,string );
string ObtenerExt(string );
void GraficarINODEYBLOCK(string ,string ,string,string,string);
void generarDotINODE(string , string , string ,int ,int ,int );

void GraficarJOURNALING(string ,string ,string ,string );
void generarDotJOURNALING(string ,string , string ,int );
void generarDotBLOCK(string , string , string , int , int , int );
void generarDotSB(string , string , string , int );
void CAT(string );
vector<string> Mkfsids;


//Variables globales
//TODO:ARREGLAR PATH 
string path="/home/eduardo/Escritorio/ArchivosVacas/Proyecto_1/Proyecto_1";
bool SesionActiva=false;

//ESTRUCTURAS GENERALES
struct ActiveSession{
    int IDU;
    int IDG;
    int PosicionInicialSB;
    int PosicionInicialJournal;
    int FS;
    char fit;
    string direccion;
};
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
    char disk_fit;
    Particion mbr_partition[4];
};
struct EBR{
    char part_status; 
    char part_fit; 
    int part_start; 
    int part_size; 
    int part_next; 
    char part_name[16]; 
};
struct DiscoD{ 
	string path="";
	int size;
	string unit="M"; /// k=kb m=mb megabytes como default
    char fit='F';//Primer ajuste como default.
}
Disk1;
struct nodoParticionMontada {
    string path;
    string name;
    char letra;
    int numero;
};
struct SuperBloque{
    int s_filesystem_type; //Guarda el numero que identifica al sistea de archivos utilizado
    int s_inodes_count; //Guarda el numero total de inodos
    int s_blocks_count; //Guarda el numero total de bloques
    int s_free_blocks_count; //Contiene el numero de bloques libres
    int s_free_inodes_count; //Contiene el numero inodos libres
    time_t s_mtime; //Ultima fecha en el que el sistema fue montado
    time_t s_umtime; //Ultima fecha en que el sistema fue desmontado
    int s_mnt_count; //Indica cuantas veces se ha montado el sistema
    int s_magic; //Valor que identifica al sistema de archivos 0xEF53
    int s_inode_size; //Tamano del Inodo
    int s_block_size; //Tamano del bloque
    int s_first_ino; //Primer Inodo libre
    int s_first_blo; //Primero bloque libre
    int s_bm_inode_start; //Guardara el inicio del bitmap de inodos
    int s_bm_block_start; //Guardara el inicio del bitmap de bloques
    int s_inode_start; //Guarada el inicio de la tabla de inodos
    int s_block_start; //Guardara el inicio de la tabla de bloques
};
struct Tabla_Inodos{
    int i_uid; //UID del usuario propiertario del archivo/carpeta
    int i_gid; //GID del grupo al que pertenece el archivo/carpeta
    int i_size; //Tamano del archivo en bytes
    time_t i_atime; //Ultima fecha en que se leyo el Inodo sin modificarlo
    time_t i_ctime; //Fecha en que se creo el el Inodo
    time_t i_mtime; //Ultima fecha en la que se modifco
    int i_block[15]; //Array de bloques
    char i_type; //Indica si es archivo o carpeta
    int i_perm; //Guarada los permisos del archivo/carpeta
    
};
struct Bloque_Archivos{
    char b_content[64];
};
struct Content{
    char b_name[12];//Nombre carpeta/archivo
    int b_inodo;//Apuntador hacia un Inodo asociado al archivo o carpeta
};
struct Bloque_Carpetas{
    Content b_content[4];//Array con el contenido de la carpeta
};
struct Journaling{
    char Journal_tipo_operacion[10];
    int Journal_tipo;//Archivo/Carpeta
    char Journal_nombre[100];
    char Journal_contenido[100];
    time_t Journal_fecha;
    int Journal_propietario;
    int Journal_permisos;
};
struct BloqueDeApuntadores{
    int b_pointers[16];//Array con los apuntadores hacia bloques
};
void CrearDisco(DiscoD op){
cout<<"\033[92m************************CREANDO DISCO*************************\033[0m\n"<<endl;
    string s = path + op.path;//url raiz + url de la entrada
    char sc[s.size() + 1];
    strcpy(sc, s.c_str());
    FILE *file=NULL;
    file=fopen(sc,"r"); //r= read = sera verdadero si el disco ya Exist
    if(file!=NULL){
        cout<<"\033[91mYa Exist el disco\033[0m"<<endl;
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
        mbr.mbr_partition[i].part_start = -1;
        strcpy(mbr.mbr_partition[i].part_name,"");
    }
    
    cout<<"\033[92mDisco nuevo :: \033[0m"<<op.path<<"\n \033[92mFecha de creacion: \033[0m"<<asctime(gmtime(&mbr.mbr_fecha_creacion))<<endl;
    cout<<"\033[92mSignature: \033[0m"<<mbr.mbr_disk_signature <<endl;
    cout<<"\033[92mTamaño: \033[0m"<<mbr.mbr_tamano <<" \033[92mBytes\033[0m"<<endl;
    cout<<"\033[92mFit:\033[0m " <<mbr.disk_fit <<endl;
    cout<<"\n\033[92m-------------DISCO CREADO--------------------\n\033[0m"<<endl;
    ///escritura del mbr
    fseek(file,0,SEEK_SET);
    fwrite(&mbr,sizeof(MBR),1,file);
    fclose(file);
}
//FUNCIONES Y METODOS A UTILIZAR 
Tabla_Inodos crearInodo(int ,char ,int );
Bloque_Carpetas crearBloqueCarpeta();
int buscarContentLibre(FILE* ,int ,Tabla_Inodos &,Bloque_Carpetas &, BloqueDeApuntadores &,int &,int &,int &);
ActiveSession SesionActual;
vector<nodoParticionMontada> arreglonodos;
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
    string llave;
    vector<string> res;

    while ((pos_end = cadena.find (delimitador, pos_start)) != string::npos) {
        llave = cadena.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(llave);
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
        cout<<"\033[92mEl archivo fue eliminado satisfactoriamente\033[0m\n"<<endl;
    }
    else{
        cout<<"\033[91mError: No se pudo eliminar el archivo\033[0m\n"<<endl;
    }
}

string EliminarComillasYreemplazarEspacios(string cadena){
    string retorno=cadena;
    char e = cadena.at(0);
    if(e=='\"'){//Si viene una comilla en el primer espacio las vamos a eliminar
        
        retorno = EliminarComillas(cadena);//Eliminamos comillas

        return retorno;//Sino retornamos solamente sin comillas

    }else{//Si no vienen comillas retornamos

        return retorno;
    }
    
}
void EjecutarComando(char comando[200]){
    char temporal[200];
    strcpy(temporal,comando);
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
            cout<<"\033[91m******       Script pausado!       ******\n"<<"******Presione Enter para continuar******\033[0m\n";
            //pausamos
            pause = cin.get();
        }
        vector<string>lineSplit = SplitSpace(comandoCasteado);//Spliteamos por espacios
        vector<string>lineSplitSinCasteo = SplitSpace(temporal);
            if(lineSplit[0]=="MKDISK"){
                int sizeerror,fiterror,uniterror =0;
                bool unit,fit = false;//Variables booleanas que se estableceran por defecto si no se declaran en el comando si son false se pondran en automatico por defecto segun enunciado
                vector<string> auxiliar;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    vector<string> auxiliarSinCastear;
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-PATH"){//Si el comando es el -path entonces entrara a esta condicional
                        string auxiliare = auxiliarSinCastear[1];
                        string path;
                        path = EliminarComillasYreemplazarEspacios(auxiliare);
                        mkcarpetas(path);//Creamos las carpetas con el path ingresado.
                        Disk1.path = path;//Asignamos el path al disco                    
                    }else if(auxiliar[0]=="-FIT"){//Asignamos el fit al disco
                        if(auxiliar[1]=="BF"){
                            Disk1.fit= 'B';
                            fit = true;
                        }else if(auxiliar[1]=="FF"){
                            Disk1.fit= 'F';
                            fit = true;
                        }else if(auxiliar[1]=="WF"){
                            Disk1.fit= 'W';
                            fit = true;
                        }else{
                            fiterror = 1;
                            cout<<" \033[91mError: Esta tratando de ingresar un ajuste no permitido\033[0m"<<endl;
                        }    
                    }
                    else if(auxiliar[0]=="-UNIT"){//Asignamos la unidad al disco
                        if(auxiliar[1]=="M" || auxiliar[1]=="K"){
                            Disk1.unit = auxiliar[1];
                            unit = true;
                        }else{
                            uniterror = 1;
                            cout<<"\033[91mError: Esta tratando de ingresar una unidad no permitida para crear disco\033[0m"<<endl;
                        }
                        
                    }
                    else if(auxiliar[0] == "-SIZE"){//Asignamos el tamanio al disco
                        if(stof(auxiliar[1])<=0){
                            sizeerror = 1;
                            cout<<"\033[91mError: El tamaño del disco no puede ser negativo o igual a cero\033[0m"<<endl;
                        }else{
                            Disk1.size = stof(auxiliar[1]);
                        }                        
                    }
                }
                if(fit==false){//Asignacion default
                    Disk1.fit = 'F';
                }
                if(unit==false){//Asignacion default
                    Disk1.unit ="M";
                }
                if(sizeerror==1 || uniterror==1 || fiterror==1){
                    cout<<"\033[91mError: No se pudo crear el disco!!\033[0m"<<endl;
                }else{
                    CrearDisco(Disk1);//Creamos disco
                }                
            }else if(lineSplit[0]=="RMDISK"){
                string dir;
                vector<string> aux;
                vector<string> auxiliarSinCastear;
                auxiliarSinCastear = Split(lineSplitSinCasteo[1],"~:~");
                aux = Split(lineSplit[1],"~:~");
                if(aux[0] == "-PATH"){
                    string auxiliare = auxiliarSinCastear[1];
                    string pathx;
                    pathx = EliminarComillasYreemplazarEspacios(auxiliare);
                    dir = path +pathx;
                    int cont = arreglonodos.size();
                    for(int i = 0; i < cont;i++){
                        if(arreglonodos[i].path == dir){
                            string vd = "vd";
                            string letra = "";
                            letra.push_back(arreglonodos[i].letra);
                            string numero = to_string(arreglonodos[i].numero);
                            string id = vd+letra+numero;
                            DesmontarParticionSinMensaje(id,cont);
                            continue;
                        }
                    }
                    borrardisco(dir); 
                }           
            }else if(lineSplit[0]=="FDISK"){
                int sizeerror,fiterror,uniterror,typeerror =0;
                int sizepart;
                char unitpart;
                string rutapart;
                char fitpart;
                string namepart; 
                char typepart;
                bool deffit,deftype,defunit=false;
                vector<string> auxiliar;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    vector<string> auxiliarSinCastear;
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    auxiliar = Split(lineSplit[i],"~:~");
                    if(auxiliar[0] == "-SIZE"){//Si el comando es el -path entonces entrara a esta condicional
                        if(stof(auxiliar[1])<=0){
                            sizeerror = 1;
                            cout<<"\033[91mError: Esta tratando de ingresar una unidad no permitida para crear particion\033[0m"<<endl;
                        }else{
                            sizepart = stof(auxiliar[1]);
                        }
                    }else if(auxiliar[0]=="-UNIT"){
                        if(auxiliar[1]=="M"){
                            unitpart = 'M';
                            defunit = true;
                        }else if(auxiliar[1]=="K"){
                            unitpart = 'K';
                            defunit = true;
                        }else if(auxiliar[1]=="B"){
                            unitpart = 'B';
                            defunit = true;
                        }else{
                            uniterror = 1;
                            cout<<"\033[91mError: Esta tratando de ingresar una unidad no permitida para crear una particion\033[0m"<<endl;
                        }    
                    }else if(auxiliar[0]=="-PATH"){
                        string auxiliare = auxiliarSinCastear[1];
                        string pathx;
                        pathx = EliminarComillasYreemplazarEspacios(auxiliare);
                        rutapart = pathx;    
                    }else if(auxiliar[0]=="-TYPE"){
                        if(auxiliar[1]=="P"){
                            typepart = 'P';
                            deftype = true;
                        }else if(auxiliar[1]=="E"){
                            typepart = 'E';
                            deftype = true;
                        }else if(auxiliar[1]=="L"){
                            typepart = 'L';
                            deftype = true;
                        }else{
                            typeerror = 1;
                            cout<<"\033[91mError: Esta tratando de ingresar un tipo incompatible con las que se pueden crear particiones utiliza P,E o L\033[0m"<<endl;
                        } 
                    }else if(auxiliar[0]=="-FIT"){//Asignamos el fit a la particion
                        if(auxiliar[1]=="BF"){
                            fitpart = 'B';
                            deffit = true;
                        }else if(auxiliar[1]=="WF"){
                            fitpart = 'W';
                            deffit = true;
                        }else if(auxiliar[1]=="FF"){
                            fitpart = 'F';
                            deffit = true;
                        }else{
                            fiterror = 1;
                            cout<<"\033[91mError: Esta tratando de ingresar un ajuste no permitido para la particion\033[0m"<<endl;
                        }    
                    }else if(auxiliar[0]=="-NAME"){
                        namepart = auxiliar[1];                       
                    }
                }
                if(!defunit){//Establecemos valor default de las unidades si no viene en el comando
                    unitpart = 'K';
                }
                if(!deffit){//Establecemos valor default de el fit si no viene en el comando.
                    fitpart = 'W';
                }
                if(!deftype || typepart=='P'){//Creamos particion primaria si no viene establecido en el comando el tipo de particion
                    //Creamos la particion primaria
                    cout<<"\033[93m-----Datos Partición PRIMARIA a Crear-----\033[0m"<<endl;
                    cout<<"\033[93msize:\033[0m "<<sizepart<<endl;
                    cout<<"\033[93munit:\033[0m "<<unitpart<<endl;
                    cout<<"\033[93mruta:\033[0m "<<rutapart<<endl;
                    cout<<"\033[93mfit:\033[0m "<<fitpart<<endl;
                    cout<<"\033[93mname:\033[0m "<<namepart<<endl;
                    ParticionPrimaria(sizepart,unitpart,rutapart,fitpart,namepart);

                }else if(deftype){
                    if(typepart=='E'){
                    cout<<"\033[93m-----Datos Partición EXTENDIDA a Crear-----\033[0m"<<endl;
                    cout<<"\033[93msize:\033[0m "<<sizepart<<endl;
                    cout<<"\033[93munit:\033[0m "<<unitpart<<endl;
                    cout<<"\033[93mruta:\033[0m "<<rutapart<<endl;
                    cout<<"\033[93mfit:\033[0m "<<fitpart<<endl;
                    cout<<"\033[93mname:\033[0m "<<namepart<<endl;
                        PaticionExtendida(sizepart,unitpart,rutapart,fitpart,namepart);
                    }else if(typepart=='L'){
                    cout<<"\033[93m-----Datos Partición LOGICA a Crear-----\033[0m"<<endl;
                    cout<<"\033[93msize:\033[0m "<<sizepart<<endl;
                    cout<<"\033[93munit:\033[0m "<<unitpart<<endl;
                    cout<<"\033[93mruta:\033[0m "<<rutapart<<endl;
                    cout<<"\033[93mfit:\033[0m "<<fitpart<<endl;
                    cout<<"\033[93mname:\033[0m "<<namepart<<endl;
                        ParticionLogica(sizepart,unitpart,rutapart,fitpart,namepart);
                    }
                }  
            }else if(lineSplit[0] == "MOUNT"){
                string ruta, nombre;
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-PATH"){//Si el comando es el -path entonces entrara a esta condicional
                        string auxiliare = auxiliarSinCastear[1];
                        string pathx;
                        pathx = EliminarComillasYreemplazarEspacios(auxiliare);
                        ruta = pathx;
                    }else if(auxiliar[0]=="-NAME"){
                        nombre = auxiliar[1];
                    }
                }
                MontarParticion(nombre,ruta);
            }else if(lineSplit[0] == "UNMOUNT"){
                string id;
                vector<string> auxiliar;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    if(auxiliar[0] == "-ID"){//Si el comando es el -path entonces entrara a esta condicional
                        id = auxiliar[1];
                    }
                }
                DesmontarParticion(id);
            }else if(lineSplit[0] == "MKFS"){
                string id;
                bool flag = true;
                string type = "FULL";
                int fs=2;
                vector<string> auxiliar;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    if(auxiliar[0] == "-ID"){
                        id = auxiliar[1];
                    }else if(auxiliar[0] == "-TYPE"){
                        if(auxiliar[1]=="FULL" || auxiliar[1] == "FAST"){
                            type = auxiliar[1];
                        }else{
                            flag = false;
                            cout<<"\033[91mError:El tipo ingresado es invalido\033[0m"<<endl;
                        }                        
                    }else if(auxiliar[0] == "-FS"){
                        auto arreglochar = auxiliar[1].c_str();
                        if((arreglochar[0]=='2')&&(arreglochar[1]=='F')&&(arreglochar[2]=='S')){
                            fs=2;
                        }else if((arreglochar[0]=='3')&&(arreglochar[1]=='F')&&(arreglochar[2]=='S')){
                            fs=3;
                        }else{
                            flag = false;
                            cout<<"\033[91mError:El fs ingresado es invalido\033[0m"<<endl;
                        }
                    }
                }
                if(flag){
                    ComandoMKFS(id,type,fs);
                }                
            }else if(lineSplit[0] == "LOGIN"){
                string id,usuario,contrasenia;
                bool flag = true;
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear;
                if(lineSplit.size()!=4){
                    flag=false;
                }
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-USR"){
                        usuario = auxiliarSinCastear[1];
                    }else if(auxiliar[0] == "-PWD"){
                        contrasenia = auxiliarSinCastear[1];
                    }else if(auxiliar[0] == "-ID"){
                        id = auxiliar[1];
                    }
                }
                if(flag){
                    login(usuario,contrasenia,id);
                }else{
                   cout<<"\033[91mError: Error en el comando \033[0m"<<endl; 
                }                
            }else if(lineSplit[0] == "LOGOUT"){
                logout();                          
            }else if(lineSplit[0] == "MKGRP"){
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-NAME"){
                        MKGRP(auxiliarSinCastear[1]);
                    }
                }              
            }else if(lineSplit[0] == "RMGRP"){
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-NAME"){
                        RMGRP(auxiliarSinCastear[1]);
                    }
                }              
            }else if(lineSplit[0] == "MKUSR"){
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear;
                string usuario;
                string pass;
                string grupo;
                bool flag=true;
                
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-USR"){
                        usuario = auxiliarSinCastear[1];
                    }else if(auxiliar[0] == "-PWD"){
                        pass = auxiliarSinCastear[1];
                    }
                    else if(auxiliar[0] == "-GRP"){
                        grupo = auxiliarSinCastear[1];
                    }
                }
                if(flag){
                    MKUSR(usuario,pass,grupo);
                }
            }else if(lineSplit[0] == "RMUSR"){
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-USR"){
                        RMUSR(auxiliarSinCastear[1]);
                    }
                }              
            }else if(lineSplit[0] == "CHMOD"){
                bool r =false;
                string path,ugo;
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear,auxiliarr;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarr = Split(lineSplit[i],"-");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-PATH"){
                        string auxiliare = auxiliarSinCastear[1];
                        string pathx;
                        pathx = EliminarComillasYreemplazarEspacios(auxiliare);
                        path = pathx;
                    }else if(auxiliar[0] == "-UGO"){
                       ugo = auxiliar[1];
                    }else if(auxiliarr[1] == "R"){
                        r = true;
                    }
                }
                CHMOD(path,ugo,r);              
            }else if(lineSplit[0] == "MKFILE"){
                bool p =false;
                string path="";
                string cont="";
                string name="";
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear,auxiliarr,namepath;
                int size;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarr = Split(lineSplit[i],"-");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-PATH"){
                        string auxiliare = auxiliarSinCastear[1];
                        string pathx;
                        pathx = EliminarComillasYreemplazarEspacios(auxiliare);

                        path = pathx;
                        namepath = Split(path,"/");
                        name = namepath[namepath.size()-1];

                    }else if(auxiliar[0] == "-SIZE"){
                       size = stof(auxiliar[1]);
                    }else if(auxiliarr[1] == "P"){
                        p = true;
                    }else if(auxiliar[0] == "-CONT"){
                       cont = auxiliar[1];
                    }
                }
                MKFILE(path,cont,size,p,name);            
            }else if(lineSplit[0] == "MKDIR"){
                bool p =false;
                string path="";
                string name="";
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear,auxiliarr,namepath;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarr = Split(lineSplit[i],"-");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-PATH"){
                        string auxiliare = auxiliarSinCastear[1];
                        string pathx;
                        pathx = EliminarComillasYreemplazarEspacios(auxiliare);

                        path = pathx;
                        namepath = Split(path,"/");
                        name = namepath[namepath.size()-1];

                    }else if(auxiliarr[1] == "P"){
                        p = true;
                    }
                }
                MKDIR(name,path,p);            
            }else if(lineSplit[0] == "REP"){
                bool p =false;
                string path="";
                string name="";
                string id="";
                string ruta="";
                vector<string> auxiliar;
                vector<string> auxiliarSinCastear,namepath;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    auxiliarSinCastear = Split(lineSplitSinCasteo[i],"~:~");
                    if(auxiliar[0] == "-NAME"){
                       /*string auxiliare = auxiliarSinCastear[1];
                        string pathx;
                        pathx = EliminarComillasYreemplazarEspacios(auxiliare);
                        path = pathx;
                        namepath = Split(path,"/");
                        name = namepath[namepath.size()-1];*/
                        name = auxiliar[1];

                    }else if(auxiliar[0] == "-PATH"){
                        string auxiliare = auxiliarSinCastear[1];
                        string pathx;
                        pathx = EliminarComillasYreemplazarEspacios(auxiliare);
                        path = pathx;
                    }else if(auxiliar[0] == "-ID"){
                        id = EliminarComillasYreemplazarEspacios(auxiliarSinCastear[1]);                       
                    }else if(auxiliar[0] == "-RUTA"){
                       string auxiliare = auxiliarSinCastear[1];
                        string pathx;
                        pathx = EliminarComillasYreemplazarEspacios(auxiliare);
                        ruta = pathx;
                    }
                }
                REPORTES(name,path,id,ruta);          
            }
    }
}
void leerscript(string ruta){//Metodo que recibe como parameto la ruta del script con comandos a ejecutar
    string linea;
    char line[200];
    ifstream file(ruta.c_str());
    while (getline(file,linea)){
        if(!linea.empty()){
            strcpy(line,linea.c_str());
            cout<<"\033[91mSCRIPT:\033[0m " << linea<<endl;
            EjecutarComando(line);
        }
    }
    /*FILE *script;
    if((script = fopen(ruta.c_str(),"r"))){//Si se encuentra el archivo y se puede abrir con exito
        char line[200]="";//declaramos un arreglo de char para poder almacenar linea individualmente 
        memset(line,0,sizeof(line));
        while(fgets(line,sizeof line,script)){//Mientras se lea una linea con salto de linea se ejecutara el while  
            if(line[0]!='\n'){//Obtenemos el dato de cada linea
                cout <<"SCRIPT: "<< line << endl;//Imprimimos el comando
                //lineas.push_back(line);//Pusheamos la linea al arreglo
                EjecutarComando(line);//Ejecutamos el comando
            }
            memset(line,0,sizeof(line));
        }
        fclose(script);//Cerramos el archivo.
    }else{
        cout << "Error al abrir el SCRIPT" << endl;
    }    */
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
bool ExisteParticion(string ruta, string name){
    int extendida = -1;
    FILE *filiPart;
    if((filiPart = fopen(ruta.c_str(),"rb+"))){
        MBR mbr;
        fseek(filiPart,0,SEEK_SET);
        fread(&mbr, sizeof(MBR),1,filiPart);
        for(int i = 0; i < 4; i++){
            if(strcmp(mbr.mbr_partition[i].part_name,name.c_str()) == 0){
                fclose(filiPart);
                return true;
            }else if(mbr.mbr_partition[i].part_type == 'E'){
                extendida = i;
            }
        }
        if(extendida != -1){
            fseek(filiPart, mbr.mbr_partition[extendida].part_start,SEEK_SET);
            EBR ebr;
            while((fread(&ebr,sizeof(EBR),1,filiPart))!=0 && (ftell(filiPart) < (mbr.mbr_partition[extendida].part_size + mbr.mbr_partition[extendida].part_start))){
                if(strcmp(ebr.part_name,name.c_str()) == 0){
                    fclose(filiPart);
                    return true;
                }
                if(ebr.part_next == -1){
                    fclose(filiPart);
                    return false;
                }
            }
        }
    }
    fclose(filiPart);
    return false;
}
//FDISK MANEJO DE PARTICIONES
void ParticionPrimaria(int size,char unit,string ruta,char fit,string name){
    char fittmp = 0;
    char unittmp = 0;
    int sizetmp;
    char buffer = '1';
    if(fit!=0){//Verificamos si no viene "vacio" el fit.
        fittmp = fit;
    }else{
        fittmp = 'W';//Peor ajuste por default
    }
    //Validamos la unidad y le damos el tamanio a la particion dependiendo de la unidad ingresada.
    if(unit!=0){
        unittmp = unit;
        if(unittmp == 'B'){
            sizetmp = size;
        }else if(unittmp == 'K'){
            sizetmp = size *1024;
        }else if(unittmp == 'M'){
            sizetmp = size * 1024 * 1024;
        }else{
            cout<<"\033[91mError: No esta ingresando una unidad permitida para la creacion de la particion primaria!!\033[0m"<<endl;
        }
    }else{
        unittmp = 'K';
        sizetmp = size * 1024;
    }

    FILE *fileprimary;
    MBR mbr;
    string rutatmp = path + ruta; 
    if((fileprimary = fopen(rutatmp.c_str(),"rb+"))){//Abrimos la ruta del disco y lo abrimos con rb+ para actualizar el archivo binario permitiendo tanto lectura como escritura 
        bool espacioDisponible = false;//Varible para saber si hay espacio para agregar la particion
        int contadorParticion = 0;//Contador para ver cuantas particiones hay.
        fseek(fileprimary,0,SEEK_SET);//Sitúa el puntero de lectura/escritura de un archivo en la posición indicada, 0 para que se situe al inicio del archivo
        fread(&mbr,sizeof(MBR),1,fileprimary);//Leemos el archivo mbr que esta dentro del disco.
        //Verificamos en el mbr si hay particiones disponibles.
        for(int i=0; i < 4; i++){
            if(mbr.mbr_partition[i].part_start == -1 || (mbr.mbr_partition[i].part_status=='1') && (mbr.mbr_partition[i].part_size>=sizetmp) ){
                espacioDisponible = true;
                contadorParticion = i;
                break;
            }
        }
        if(espacioDisponible){//verificamos si el espacio si es suficiente o no
            int bytes=0;
            for(int i=0; i < 4; i++){
                if(mbr.mbr_partition[i].part_status!='1'){//verificamos que las particiones no esten en estado activo
                    bytes += mbr.mbr_partition[i].part_size;//Sumamos el espacio disponible en las particiones NO ACTIVAS
                }
            }
            cout<<"\033[96mEspacio Disponible: "<<mbr.mbr_tamano-bytes << " Bytes\033[0m" <<endl;
            cout<<"\033[96mEspacio Necesario para particion primaria: "<<sizetmp<<" Bytes\033[0m"<<endl;
            if((mbr.mbr_tamano-bytes)>=sizetmp){//Verificamos si el tamanio es suficiente para poder aniadir la particion
                if(!ExisteParticion(rutatmp,name)){
                    if(mbr.disk_fit=='F'){//First Fit
                        mbr.mbr_partition[contadorParticion].part_type = 'P';
                        mbr.mbr_partition[contadorParticion].part_fit = fittmp;
                        if(contadorParticion==0){
                            mbr.mbr_partition[contadorParticion].part_start = sizeof(mbr);
                        }else{
                            mbr.mbr_partition[contadorParticion].part_start=mbr.mbr_partition[contadorParticion-1].part_start+mbr.mbr_partition[contadorParticion-1].part_size;
                        }
                        mbr.mbr_partition[contadorParticion].part_size = sizetmp;
                        mbr.mbr_partition[contadorParticion].part_status = '0';
                        strcpy(mbr.mbr_partition[contadorParticion].part_name,name.c_str());
                        fseek(fileprimary,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,fileprimary);//Guardamos en el nuevo mbr
                        fseek(fileprimary,mbr.mbr_partition[contadorParticion].part_start,SEEK_SET);//Guardamos los bytes de la particion
                        for(int i=0; i < sizetmp; i++){
                            fwrite(&buffer,1,1,fileprimary);
                        }
                        cout<<"\n \033[96mParticion PRIMARIA creada con exito de tipo FIRST FIT!!!\033[0m"<<endl;
                    }else if(mbr.disk_fit=='B'){//Best Fit
                        int best = contadorParticion;
                        for(int i = 0; i < 4; i++){
                            if(mbr.mbr_partition[i].part_start == -1 || (mbr.mbr_partition[i].part_status == '1' && mbr.mbr_partition[i].part_size>=sizetmp)){
                                if(i != contadorParticion){
                                    if(mbr.mbr_partition[best].part_size > mbr.mbr_partition[i].part_size){
                                        best = i;
                                        break;
                                    }
                                }
                            }
                        }
                        mbr.mbr_partition[best].part_type='P';
                        mbr.mbr_partition[best].part_fit = fittmp;
                        //Incio de particion
                        if(best==0){//Primera posicion
                            mbr.mbr_partition[best].part_start = sizeof(mbr);
                        }else{
                            mbr.mbr_partition[best].part_start = mbr.mbr_partition[best-1].part_start + mbr.mbr_partition[best-1].part_size;//Iniciamos la particion en la primera posicion vacia.
                        }
                        mbr.mbr_partition[best].part_size = sizetmp;
                        mbr.mbr_partition[best].part_status = '0';//Estado Activado.
                        strcpy(mbr.mbr_partition[best].part_name,name.c_str());//Renombramos
                        //Guardamos en MBR new
                        fseek(fileprimary,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,fileprimary);//Guardamos en el nuevo mbr
                        fseek(fileprimary,mbr.mbr_partition[best].part_start,SEEK_SET);//Guardamos los bytes de la particion
                        for(int i=0; i < sizetmp; i++){
                            fwrite(&buffer,1,1,fileprimary);
                        }
                        cout<<"\n \033[96mParticion PRIMARIA creada con exito de tipo BEST FIT!!!\033[0m"<<endl;
                    }else if(mbr.disk_fit == 'W'){//WORST FIT
                        int  worst= contadorParticion;
                        for(int i = 0; i < 4; i++){
                            if(mbr.mbr_partition[i].part_start == -1 || (mbr.mbr_partition[i].part_status == '1' && mbr.mbr_partition[i].part_size>=sizetmp)){
                                if(i != contadorParticion){
                                    if(mbr.mbr_partition[worst].part_size < mbr.mbr_partition[i].part_size){
                                        worst = i;
                                        break;
                                    }
                                }
                            }
                        }
                        mbr.mbr_partition[worst].part_type = 'P';
                        mbr.mbr_partition[worst].part_fit = fittmp;
                        //Inicio de la particion
                        if(worst == 0){//Por si es la primera opcion
                            mbr.mbr_partition[worst].part_start = sizeof(mbr);
                        }else{
                            mbr.mbr_partition[worst].part_start = mbr.mbr_partition[worst-1].part_start + mbr.mbr_partition[worst-1].part_size;
                        }
                        mbr.mbr_partition[worst].part_size = sizetmp;
                        mbr.mbr_partition[worst].part_status = '0';//Activamos la particion
                        strcpy(mbr.mbr_partition[worst].part_name,name.c_str());
                        //Guardamos en MBR nes
                        fseek(fileprimary,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,fileprimary);//Guardamos en el nuevo mbr
                        fseek(fileprimary,mbr.mbr_partition[worst].part_start,SEEK_SET);//Guardamos los bytes de la particion
                        for(int i = 0; i < sizetmp; i++){
                            fwrite(&buffer,1,1,fileprimary);
                        }
                        cout <<"\n \033[96mParticion PRIMARIA creada con exito de tipo WORST FIT!!!\033[0m"<<endl;
                    }
                
                }else{
                    cout<<"\033[91mError: La particion ya Exist!!\033[0m"<<endl;
                }  
            }else{
                cout<<"\033[91mError: No hay tanto espacio disponible, intenta con una particion que no exceda el espacio libre\033[0m"<<endl;
            }
        }else{
            cout<<"\033[91mError: Ya se posee el numero maximo de particiones que son 4, elimina una para poder crear otra particion primaria!!\033[0m"<<endl;
        }
        fclose(fileprimary);//Cerramos disco.
    }else{
        cout<<"\033[91mError: No se pudo abrir el disco, es posible que no exista!!\033[0m"<<endl;
    }
}
void PaticionExtendida(int size,char unit,string ruta,char fit,string name){
    char fitpartExtend = 0;
    char unitpartExtend = 0;
    string rutacompletaExtend = path + ruta;
    int BytesExtend;
    char buffer = '1';
    if(fit != 0){
        fitpartExtend = fit;

    }else{
        fitpartExtend = 'W';
    }
    if(unit != 0){
        unitpartExtend = unit;
        if(unitpartExtend == 'B'){
            BytesExtend = size;
        }else if(unitpartExtend == 'K'){
            BytesExtend = size * 1024;
        }else if (unitpartExtend=='M'){
            BytesExtend = size*1024*1024;
        }
    }else{
        BytesExtend = size * 1024;
    }

    FILE *fileDisk;
    MBR mbrE;
    if((fileDisk = fopen(rutacompletaExtend.c_str(), "rb+"))){
        bool PartDisponible,ExisteExtendida = false;//Variables para verificar si hay particion disponible en disco y para verificar si hay una particion extendida
        int ContadorPart = 0;
        fseek(fileDisk,0,SEEK_SET);
        fread(&mbrE,sizeof(MBR),1,fileDisk);
        for(int i = 0; i < 4; i++){
            if (mbrE.mbr_partition[i].part_type == 'E'){
                ExisteExtendida = true;
                break;
            }
        }
        if(!ExisteExtendida){
            for(int i = 0; i < 4; i++){
                if(mbrE.mbr_partition[i].part_start == -1 || (mbrE.mbr_partition[i].part_status == '1' && mbrE.mbr_partition[i].part_size>=BytesExtend)){
                    PartDisponible = true;
                    ContadorPart = i;
                    break;
                }
            }
            if(PartDisponible){
                int espacioUsado = 0;
                for(int i = 0; i < 4; i++){
                    if(mbrE.mbr_partition[i].part_status!='1'){
                       espacioUsado += mbrE.mbr_partition[i].part_size;
                    }
                }
                cout << "\033[96mEspacio disponible para partición extendida: " << (mbrE.mbr_tamano - espacioUsado) <<" Bytes\033[0m"<< endl;
                cout << "\033[96mEspacio necesario para partición extendida:  " << BytesExtend << " Bytes\033[0m" << endl;
                if((mbrE.mbr_tamano - espacioUsado) >= BytesExtend){
                    if(!(ExisteParticion(rutacompletaExtend,name))){
                        if(mbrE.disk_fit == 'F'){
                            mbrE.mbr_partition[ContadorPart].part_type = 'E';
                            mbrE.mbr_partition[ContadorPart].part_fit = fitpartExtend;
                            if(ContadorPart == 0){
                                mbrE.mbr_partition[ContadorPart].part_start = sizeof(mbrE);
                            }else{
                                mbrE.mbr_partition[ContadorPart].part_start =  mbrE.mbr_partition[ContadorPart-1].part_start + mbrE.mbr_partition[ContadorPart-1].part_size;
                            }
                            mbrE.mbr_partition[ContadorPart].part_size = BytesExtend;
                            mbrE.mbr_partition[ContadorPart].part_status = '0';
                            strcpy(mbrE.mbr_partition[ContadorPart].part_name,name.c_str());
                            fseek(fileDisk,0,SEEK_SET);
                            fwrite(&mbrE,sizeof(MBR),1,fileDisk);
                            fseek(fileDisk, mbrE.mbr_partition[ContadorPart].part_start,SEEK_SET);
                            EBR ebr;
                            ebr.part_fit = fitpartExtend;
                            ebr.part_status = '0';
                            ebr.part_start = mbrE.mbr_partition[ContadorPart].part_start;
                            ebr.part_size = 0;
                            ebr.part_next = -1;
                            strcpy(ebr.part_name, "");
                            fwrite(&ebr,sizeof (EBR),1,fileDisk);
                            for(int i = 0; i < (BytesExtend - (int)sizeof(EBR)); i++){
                                fwrite(&buffer,1,1,fileDisk);
                            }
                            cout << "\n \033[96mParticion EXTENDIDA creada con exito de tipo FIRST FIT\033[0m"<< endl;
                        }else if(mbrE.disk_fit == 'B'){
                            int MejorPosicion = ContadorPart;
                            for(int i = 0; i < 4; i++){
                                if(mbrE.mbr_partition[i].part_start == -1 || (mbrE.mbr_partition[i].part_status == '1' && mbrE.mbr_partition[i].part_size>=BytesExtend)){
                                    if(i != ContadorPart){
                                        if(mbrE.mbr_partition[MejorPosicion].part_size > mbrE.mbr_partition[i].part_size){
                                            MejorPosicion = i;
                                            break;
                                        }
                                    }
                                }
                            }
                            mbrE.mbr_partition[MejorPosicion].part_type = 'E';
                            mbrE.mbr_partition[MejorPosicion].part_fit = fitpartExtend;
                            if(MejorPosicion == 0){
                                mbrE.mbr_partition[MejorPosicion].part_start = sizeof(mbrE);
                            }else{
                                mbrE.mbr_partition[MejorPosicion].part_start =  mbrE.mbr_partition[MejorPosicion-1].part_start + mbrE.mbr_partition[MejorPosicion-1].part_size;
                            }
                            mbrE.mbr_partition[MejorPosicion].part_size = BytesExtend;
                            mbrE.mbr_partition[MejorPosicion].part_status = '0';
                            strcpy(mbrE.mbr_partition[MejorPosicion].part_name,name.c_str());
                            fseek(fileDisk,0,SEEK_SET);
                            fwrite(&mbrE,sizeof(MBR),1,fileDisk);
                            fseek(fileDisk, mbrE.mbr_partition[MejorPosicion].part_start,SEEK_SET);
                            EBR ebr;
                            ebr.part_fit = fitpartExtend;
                            ebr.part_status = '0';
                            ebr.part_start = mbrE.mbr_partition[MejorPosicion].part_start;
                            ebr.part_size = 0;
                            ebr.part_next = -1;
                            strcpy(ebr.part_name, "");
                            fwrite(&ebr,sizeof (EBR),1,fileDisk);
                            for(int i = 0; i < (BytesExtend - (int)sizeof(EBR)); i++){
                                fwrite(&buffer,1,1,fileDisk);
                            }
                            cout<< "\n \033[96mParticion EXTENDIDA creada con exito de tipo BEST FIT\033[0m"<< endl;
                        }else if(mbrE.disk_fit == 'W'){
                            int  PeorPosicion= ContadorPart;
                            for(int i = 0; i < 4; i++){
                                if(mbrE.mbr_partition[i].part_start == -1 || (mbrE.mbr_partition[i].part_status == '1' && mbrE.mbr_partition[i].part_size>=BytesExtend)){
                                    if(i != ContadorPart){
                                        if(mbrE.mbr_partition[PeorPosicion].part_size < mbrE.mbr_partition[i].part_size){
                                            PeorPosicion = i;
                                            break;
                                        }
                                    }
                                }
                            }
                            mbrE.mbr_partition[PeorPosicion].part_type = 'E';
                            mbrE.mbr_partition[PeorPosicion].part_fit = fitpartExtend;
                            if(PeorPosicion == 0){
                                mbrE.mbr_partition[PeorPosicion].part_start = sizeof(mbrE);
                            }else{
                                mbrE.mbr_partition[PeorPosicion].part_start =  mbrE.mbr_partition[PeorPosicion-1].part_start + mbrE.mbr_partition[PeorPosicion-1].part_size;
                            }
                            mbrE.mbr_partition[PeorPosicion].part_size = BytesExtend;
                            mbrE.mbr_partition[PeorPosicion].part_status = '0';
                            strcpy(mbrE.mbr_partition[PeorPosicion].part_name,name.c_str());
                            fseek(fileDisk,0,SEEK_SET);
                            fwrite(&mbrE,sizeof(MBR),1,fileDisk);
                            fseek(fileDisk, mbrE.mbr_partition[PeorPosicion].part_start,SEEK_SET);
                            EBR ebr;
                            ebr.part_fit = fitpartExtend;
                            ebr.part_status = '0';
                            ebr.part_start = mbrE.mbr_partition[PeorPosicion].part_start;
                            ebr.part_size = 0;
                            ebr.part_next = -1;
                            strcpy(ebr.part_name, "");
                            fwrite(&ebr,sizeof (EBR),1,fileDisk);
                            for(int i = 0; i < (BytesExtend - (int)sizeof(EBR)); i++){
                                fwrite(&buffer,1,1,fileDisk);
                            }
                            cout<< "\n \033[96mParticion EXTENDIDA creada con exito de tipo WORST FIT\033[0m"<< endl;
                        }
                    }else{
                        cout << "\033[91mError: La particion ya Exist!!\033[0m" << endl;
                    }
                }else{
                    cout << "\033[91mError: No hay tanto espacio disponible, intenta con una particion que no exceda el espacio libre\033[0m" << endl;
                }
            }else{
                cout << "\033[91mError: Ya se posee el numero maximo de particiones que son 4, elimina una para poder crear particion lógica!!\033[0m" << endl;
            }
        }else{
            cout << "\033[91mError: Ya Exist una partición de tipo EXTENDIDA\033[0m" << endl;
        }
    fclose(fileDisk);
    }else{
        cout << "\033[91mError: No se pudo abrir el disco, es posible que no exista!!\033[0m" << endl;
    }
}
void ParticionLogica(int size,char unit,string ruta,char fit,string name){
    char fitPartLogic = 0;
    char UnitPartLogic = 0;
    string Rutacompleta = path + ruta;
    int tamLogic;
    char buffer = '1';

    if(fit != 0)
        fitPartLogic = fit;
    else
        fitPartLogic = 'W';
    if(unit != 0){
        UnitPartLogic = unit;
        if(UnitPartLogic == 'B'){
            tamLogic = size;
        }else if(UnitPartLogic == 'K'){
            tamLogic = size * 1024;
        }else if(UnitPartLogic=='M'){
            tamLogic = size*1024*1024;
        }
    }else{
        tamLogic = size * 1024;
    }

    FILE *filelogic;
    MBR mbr;
    if((filelogic = fopen(Rutacompleta.c_str(), "rb+"))){
        int PosicionExtendida = -1;
        fseek(filelogic,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,filelogic);
        for(int i = 0; i < 4; i++){
            if(mbr.mbr_partition[i].part_type == 'E'){
                PosicionExtendida = i;
                break;
            }
        }
        if(!ExisteParticion(Rutacompleta,name)){
            if(PosicionExtendida != -1){
                EBR ebr;
                fseek(filelogic,mbr.mbr_partition[PosicionExtendida].part_start,SEEK_SET);
                fread(&ebr, sizeof(EBR),1,filelogic);
                if(ebr.part_size == 0){
                    if(mbr.mbr_partition[PosicionExtendida].part_size < tamLogic){
                        cout << "\033[91mError: No hay suficiente espacio en la Extendida para esta partición lógica.\033[0m" << endl;
                    }else{
                        ebr.part_status = '0';
                        ebr.part_fit = fitPartLogic;
                        ebr.part_start = ftell(filelogic) - sizeof(EBR);
                        ebr.part_size = tamLogic;
                        ebr.part_next = -1;
                        strcpy(ebr.part_name, name.c_str());
                        fseek(filelogic, mbr.mbr_partition[PosicionExtendida].part_start ,SEEK_SET);
                        fwrite(&ebr,sizeof(EBR),1,filelogic);
                        cout << "\n\033[96m Particion LOGICA creada con exito!!!\033[0m "<< endl;
                    }
                }else{
                    while((ebr.part_next != -1) && (ftell(filelogic) < (mbr.mbr_partition[PosicionExtendida].part_size + mbr.mbr_partition[PosicionExtendida].part_start))){
                        fseek(filelogic,ebr.part_next,SEEK_SET);
                        fread(&ebr,sizeof(EBR),1,filelogic);
                    }
                    int espacioNecesario = ebr.part_start + ebr.part_size + tamLogic;
                    if(espacioNecesario <= (mbr.mbr_partition[PosicionExtendida].part_size + mbr.mbr_partition[PosicionExtendida].part_start)){
                        ebr.part_next = ebr.part_start + ebr.part_size;
                        fseek(filelogic,ftell(filelogic) - sizeof (EBR),SEEK_SET);
                        fwrite(&ebr, sizeof(EBR),1 ,filelogic);
                        fseek(filelogic,ebr.part_start + ebr.part_size, SEEK_SET);
                        ebr.part_status = 0;
                        ebr.part_fit = fitPartLogic;
                        ebr.part_start = ftell(filelogic);
                        ebr.part_size = tamLogic;
                        ebr.part_next = -1;
                        strcpy(ebr.part_name,name.c_str());
                        fwrite(&ebr,sizeof(EBR),1,filelogic);
                        cout << "\033[96m Particion LOGICA creada con exito \033[0m"<< endl;
                    }else{
                        cout << "\033[91mError: Espacio insuficiente, la lógica sobrepasa el espacio disponible!!\033[0m" << endl;
                    }
                }
            }else{
                cout << "\033[91mError: No se puede crear partición lógica sin extendida, crear partición EXTENDIDA antes.\033[0m" << endl;
            }
        }else{
            cout << "\033[91mError: Ya Exist una partición con el mismo nombre, cambielo e intente de nuevo.\033[0m" << endl;
        }

    fclose(filelogic);
    }else{
        cout << "\033[91mError: No se ha encontrado el disco, es probable que no exista.\033[0m" << endl;
    }

}

int FindLetra(string name, string path){
    int letra = 'a';
    if(arreglonodos.size()==0){//Si esta vacio
        return letra;//retornamos a;
    }else if(arreglonodos.size()!=0){//Si no esta vacio
        for(int i = 0; i < arreglonodos.size();i++){
            if((arreglonodos[i].path==path)&&(arreglonodos[i].name==name)){
                return -1;
            }else{
                if(arreglonodos[i].path == path){
                    return arreglonodos[i].letra;
                }else if(letra <= arreglonodos[i].letra){
                    letra++;
                }
            }
        }
    }
    return letra; 
}
int FindNumero(string name,string path){
    int numero = 1;
    for(int i = 0; i < arreglonodos.size();i++){
        if((arreglonodos[i].numero == numero) && arreglonodos[i].path == path){
            numero++;
        }
    }
    return numero;
}

void ImprimirParticinesMontadas(){
    cout<<"\033[96m***** Particiones Montadas *****"<<endl;
    cout<<"***    Nombre  |    ID       ***\033[0m"<<endl;
    for(int i=0;i < arreglonodos.size();i++){
        cout<<"***  "<<arreglonodos[i].name<<"  |  vd"<<arreglonodos[i].letra<<arreglonodos[i].numero<<endl;
        cout<<"********************************"<<endl;
    }
}

void MontarParticion(string name, string pathr){
    string pathcompleta = path + pathr;
    int NumeroPrimariaOrExtendida = FindPrimariaYExtendida(pathcompleta,name);
    if(NumeroPrimariaOrExtendida != -1){
        FILE *filedisk;
        if((filedisk = fopen(pathcompleta.c_str(),"rb+"))){
            MBR mbr;
            fseek(filedisk, 0, SEEK_SET);
            fread(&mbr, sizeof(MBR),1,filedisk);
            mbr.mbr_partition[NumeroPrimariaOrExtendida].part_status = '2';
            fseek(filedisk,0,SEEK_SET);
            fwrite(&mbr,sizeof(MBR),1,filedisk);
            fclose(filedisk);
            int letra = FindLetra(name,pathcompleta);
            if(letra == -1){
                cout << "\033[31mError: la particion ya esta montada.\033[0m" << endl;
            }else{
                int num = FindNumero(name,pathcompleta);
                char LetraCasteada = static_cast<char>(letra);
                string id = "vd";
                id += LetraCasteada + to_string(num);
                nodoParticionMontada newNodo;
                newNodo.path = pathcompleta;
                newNodo.name = name;
                newNodo.letra = LetraCasteada;
                newNodo.numero = num;
                arreglonodos.push_back(newNodo);
                cout << "\033[94mParticion montada con exito.\033[0m" << endl;
                ImprimirParticinesMontadas();
            }
        }else{
            cout << "\033[31mErro: no se encuentra el disco.\033[0m" << endl;
        }
    }else{//Posiblemente logica
        int NumeroLogica = FindLogic(pathcompleta,name);
        if(NumeroLogica != -1){
            FILE *filedisk;
            if((filedisk = fopen(pathcompleta.c_str(), "rb+"))){
                EBR ebr;
                fseek(filedisk, NumeroLogica, SEEK_SET);
                fread(&ebr, sizeof(EBR),1,filedisk);
                ebr.part_status = '2';
                fseek(filedisk,NumeroLogica,SEEK_SET);
                fwrite(&ebr,sizeof(EBR),1, filedisk);
                fclose(filedisk);
                int letra = FindLetra(name,pathcompleta);
                if(letra == -1){
                    cout << "\033[31mERROR: La particion ya esta montada.\033[0m" << endl;
                }else{
                    int num = FindNumero(name,pathcompleta);
                    char LetraCasteada = static_cast<char>(letra);
                    string id = "vd";
                    id += LetraCasteada + to_string(num);
                    nodoParticionMontada newNodo;
                    newNodo.path = pathcompleta;
                    newNodo.name = name;
                    newNodo.letra = LetraCasteada;
                    newNodo.numero = num;
                    arreglonodos.push_back(newNodo);
                    cout << "\033[94mParticion montada con exito.\033[0m" << endl;
                    ImprimirParticinesMontadas();
                }
            }else{
                cout << "\033[31mERROR: no se encuentra el disco.\033[0m" << endl;
            }
        }else{
            cout << "\033[31mERROR: no se encuentra la particion a montar.\033[0m" << endl;
        }
    }
}

int FindPrimariaYExtendida(string direccion, string nombre){
    string auxPath = direccion;
    string auxName = nombre;
    FILE *filedisk;
    if((filedisk = fopen(auxPath.c_str(),"rb+"))){
        MBR mbr;
        fseek(filedisk,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,filedisk);
        for(int i = 0; i < 4; i++){
            if(mbr.mbr_partition[i].part_status != '1'){
                if(strcmp(mbr.mbr_partition[i].part_name,auxName.c_str()) == 0){
                    return i;
                }
            }
        }

    }
    return -1;
}

int FindLogic(string direccion, string nombre){
    string auxPath = direccion;
    string auxName = nombre;
    FILE *filedisk;
    if((filedisk = fopen(auxPath.c_str(),"rb+"))){
        int extendida = -1;
        MBR mbr;
        fseek(filedisk,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,filedisk);
        for(int i = 0; i < 4; i++){
            if(mbr.mbr_partition[i].part_type == 'E'){
                extendida = i;
                break;
            }
        }
        if(extendida != -1){
            EBR ebr;
            fseek(filedisk, mbr.mbr_partition[extendida].part_start,SEEK_SET);
            while(fread(&ebr,sizeof(EBR),1,filedisk)!=0 && (ftell(filedisk) < mbr.mbr_partition[extendida].part_start + mbr.mbr_partition[extendida].part_size)){
                if(strcmp(ebr.part_name, auxName.c_str()) == 0){
                    return (ftell(filedisk) - sizeof(EBR));
                }
            }
        }
        fclose(filedisk);
    }
    return -1;
}

void DesmontarParticion(string id){
    string idtmp;
    bool flag=false;
    for(int i=0;i < arreglonodos.size();i++){
        string vd="VD";
        string letra="";
        letra = toupper(arreglonodos[i].letra);
        string numero = to_string(arreglonodos[i].numero);
        idtmp = vd+letra+numero;
        if(idtmp==id){ 
            arreglonodos.erase(arreglonodos.begin()+ i);  
            cout<<"\033[92m Partición desmontada con éxito. \033[0m"<<endl;
            ImprimirParticinesMontadas();
            flag = true;
            break;    
        }
    }
    if(!flag){
        cout<<"\033[91m Error: No se encontro la partición que desea desmontar!!!\n Las particiones montadas son: \033[0m"<<endl;
        ImprimirParticinesMontadas();
    }
}
void DesmontarParticionSinMensaje(string id,int cont){
    string idtmp;
    for(int i=0;i < cont;i++){
        string vd="vd";
        string letra="";
        letra = arreglonodos[i].letra;
        string numero = to_string(arreglonodos[i].numero);
        idtmp = vd+letra+numero;
        if(idtmp==id){ 
            arreglonodos.erase(arreglonodos.begin()+ i);    
        }
    }
}

void FormatEXT2(int inicio ,int tamano,string direccion){
    double n = 1+(tamano - static_cast<int>(sizeof(SuperBloque))-1)/(4+static_cast<int>(sizeof(Tabla_Inodos)) +3*static_cast<int>(sizeof(Bloque_Carpetas)));
    int ContadorEstructuras = static_cast<int>(floor(n));//Calculo para el numero de inodos
    int ContadorBloques = 3*ContadorEstructuras;
    //Asignamo todos los parametros del SB bloque
    SuperBloque SBlock;
    SBlock.s_filesystem_type = 2;
    SBlock.s_inodes_count = ContadorEstructuras;
    SBlock.s_blocks_count = ContadorBloques;
    SBlock.s_free_blocks_count = ContadorBloques -2;
    SBlock.s_free_inodes_count = ContadorEstructuras -2;
    SBlock.s_mtime = time(nullptr);//como aun no se ha montado
    SBlock.s_umtime = 0;
    SBlock.s_mnt_count = 0;
    SBlock.s_magic = 0xEF53;
    SBlock.s_inode_size = sizeof(Tabla_Inodos);
    SBlock.s_block_size = sizeof(Bloque_Archivos);
    SBlock.s_first_ino = 2;
    SBlock.s_first_blo = 2;
    //EStablecemos inicios de las diferente estructuras tanto de bitmaps como de inodos
    SBlock.s_bm_inode_start = inicio + static_cast<int>(sizeof(SuperBloque));
    SBlock.s_bm_block_start = inicio + static_cast<int>(sizeof(SuperBloque)) + ContadorEstructuras;
    SBlock.s_inode_start = inicio + static_cast<int>(sizeof (SuperBloque)) + ContadorEstructuras + ContadorBloques;
    SBlock.s_block_start = inicio + static_cast<int>(sizeof(SuperBloque)) + ContadorEstructuras + ContadorBloques + (static_cast<int>(sizeof(Tabla_Inodos))*ContadorEstructuras);
    //Declaramos la tabla de nodos y el bloque de carpetas
    Tabla_Inodos InodoBlock;
    Bloque_Carpetas Block;

    char buffer = '0';
    char buffer2 = '1';
    char buffer3 = '2';

    FILE *fileDisk = fopen(direccion.c_str(),"rb+");
    fseek(fileDisk,inicio,SEEK_SET);//Establecemos la posicion de donde escribiremos
    fwrite(&SBlock,sizeof(SuperBloque),1,fileDisk);//Escribimos el superbloque
    for(int i = 0; i < ContadorEstructuras; i++){
        fseek(fileDisk,SBlock.s_bm_inode_start + i,SEEK_SET);//Establecemos posicion
        fwrite(&buffer,sizeof(char),1,fileDisk);//Escribimos el bitmap de inodos en todas las estructuras
    }
    fseek(fileDisk,SBlock.s_bm_inode_start,SEEK_SET);
    fwrite(&buffer2,sizeof(char),1,fileDisk);// llenamos para users
    fwrite(&buffer2,sizeof(char),1,fileDisk);
    for(int i = 0; i < ContadorBloques; i++){
        fseek(fileDisk,SBlock.s_bm_block_start + i,SEEK_SET);//Establecemos posicion
        fwrite(&buffer,sizeof(char),1,fileDisk);//Escibimos los bloques
    }
    fseek(fileDisk,SBlock.s_bm_block_start,SEEK_SET);
    fwrite(&buffer2,sizeof(char),1,fileDisk);
    fwrite(&buffer3,sizeof(char),1,fileDisk);// llenamos para users

    //Llenamos para el usuario root
    InodoBlock.i_uid = 1;
    InodoBlock.i_gid = 1;
    InodoBlock.i_size = 0;
    InodoBlock.i_atime = time(nullptr);
    InodoBlock.i_ctime = time(nullptr);
    InodoBlock.i_mtime = time(nullptr);
    InodoBlock.i_block[0] = 0;
    for(int i = 1; i < 15;i++)
        InodoBlock.i_block[i] = -1;
    InodoBlock.i_type = '0';
    InodoBlock.i_perm = 664;
    fseek(fileDisk,SBlock.s_inode_start,SEEK_SET);
    fwrite(&InodoBlock,sizeof(Tabla_Inodos),1,fileDisk);
    //Copiamos para el bloque de la carpeta root con la carpeta actual
    strcpy(Block.b_content[0].b_name,".");
    Block.b_content[0].b_inodo=0;   
    //Ahora para la capeta padre
    strcpy(Block.b_content[1].b_name,"..");
    Block.b_content[1].b_inodo=0;
    //Copiamos al archivo
    strcpy(Block.b_content[2].b_name,"users.txt");
    Block.b_content[2].b_inodo=1;

    strcpy(Block.b_content[3].b_name,".");
    Block.b_content[3].b_inodo=-1;
    fseek(fileDisk,SBlock.s_block_start,SEEK_SET);
    fwrite(&Block,sizeof(Bloque_Carpetas),1,fileDisk);
    //Inodo para los usuarios del archivo txt
    InodoBlock.i_uid = 1;
    InodoBlock.i_gid = 1;
    InodoBlock.i_size = 27;
    InodoBlock.i_atime = time(nullptr);
    InodoBlock.i_ctime = time(nullptr);
    InodoBlock.i_mtime = time(nullptr);
    InodoBlock.i_block[0] = 1;
    for(int i = 1; i < 15;i++){
        InodoBlock.i_block[i] = -1;
    }
    InodoBlock.i_type = '1';
    InodoBlock.i_perm = 755;
    fseek(fileDisk,SBlock.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
    fwrite(&InodoBlock,sizeof(Tabla_Inodos),1,fileDisk);
    //Bloque de archivos para el usuario root
    Bloque_Archivos archivo;
    memset(archivo.b_content,0,sizeof(archivo.b_content));
    strcpy(archivo.b_content,"1,G,root\n1,U,root,root,123\n");
    fseek(fileDisk,SBlock.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas)),SEEK_SET);
    fwrite(&archivo,sizeof(Bloque_Archivos),1,fileDisk);
    
    cout << "\033[96mFormateo de tipo EXT2 realizada con éxito\033[0m" << endl;
    fclose(fileDisk);//Cerramos el disco
}

void FormatEXT3(int inicio,int tamano ,string direccion){
    double n = 1+(tamano - static_cast<int>(sizeof(SuperBloque))-1)/(4 + static_cast<int>(sizeof(Journaling))+static_cast<int>(sizeof(Tabla_Inodos)) +3*static_cast<int>(sizeof(Bloque_Carpetas)));
    int ContadorEstructuras = static_cast<int>(floor(n));//Este tendra el bitmap de inodos
    int ContadorBloques = 3*ContadorEstructuras;//Este llevara el bitmap de bloques
    int super_size = static_cast<int>(sizeof(SuperBloque));
    int journal_size = static_cast<int>(sizeof(Journaling))*ContadorEstructuras;

    SuperBloque SBlock;
    SBlock.s_filesystem_type = 3;
    SBlock.s_inodes_count = ContadorEstructuras;
    SBlock.s_blocks_count = ContadorBloques;
    SBlock.s_free_blocks_count = ContadorBloques - 2;
    SBlock.s_free_inodes_count = ContadorEstructuras - 2;
    SBlock.s_mtime = time(nullptr);
    SBlock.s_umtime = 0;
    SBlock.s_mnt_count = 0;
    SBlock.s_magic = 0xEF53;
    SBlock.s_inode_size = sizeof(Tabla_Inodos);
    SBlock.s_block_size = sizeof(Bloque_Archivos);
    SBlock.s_first_ino = 2;
    SBlock.s_first_blo = 2;
    SBlock.s_bm_inode_start = inicio + super_size + journal_size;
    SBlock.s_bm_block_start = inicio + super_size + journal_size + ContadorEstructuras;
    SBlock.s_inode_start = inicio + super_size + journal_size + ContadorEstructuras + ContadorBloques;
    SBlock.s_block_start = inicio + super_size + journal_size + ContadorEstructuras + ContadorBloques + static_cast<int>(sizeof(Tabla_Inodos))*ContadorEstructuras;

    Tabla_Inodos InodoBlock;
    Bloque_Carpetas Block;

    char buffer = '0';
    char buffer2 = '1';
    char buffer3 = '2';

    FILE *fileDisk = fopen(direccion.c_str(),"rb+");

    //Posicionamos y escribimos para el superbloque 
    fseek(fileDisk,inicio,SEEK_SET);
    fwrite(&SBlock,sizeof(SuperBloque),1,fileDisk);
    //Recorremos todas las estructuras y escribimos para los bitmaps de inodos
    for(int i = 0; i < ContadorEstructuras; i++){
        fseek(fileDisk,SBlock.s_bm_inode_start + i,SEEK_SET);
        fwrite(&buffer,sizeof(char),1,fileDisk);
    }
    //Escribimos el bit para el root / y para users txt
    fseek(fileDisk,SBlock.s_bm_inode_start,SEEK_SET);
    fwrite(&buffer2,sizeof(char),1,fileDisk);
    fwrite(&buffer2,sizeof(char),1,fileDisk);
    //Recorremos los bloques y escribimos los bitmaps de los mismos
    for(int i = 0; i < ContadorBloques; i++){
        fseek(fileDisk,SBlock.s_bm_block_start + i,SEEK_SET);
        fwrite(&buffer,sizeof(char),1,fileDisk);
    }
    //bitmap para root / y para users txt
    fseek(fileDisk,SBlock.s_bm_block_start,SEEK_SET);
    fwrite(&buffer2,sizeof(char),1,fileDisk);
    fwrite(&buffer3,sizeof(char),1,fileDisk);
    //Inodo para la carpeta raiz del root
    InodoBlock.i_uid = 1;
    InodoBlock.i_gid = 1;
    InodoBlock.i_size = 0;
    InodoBlock.i_atime = time(nullptr);
    InodoBlock.i_ctime = time(nullptr);
    InodoBlock.i_mtime = time(nullptr);
    InodoBlock.i_block[0] = 0;
    for(int i = 1; i < 15;i++){
        InodoBlock.i_block[i] = -1;
    }
    InodoBlock.i_type = '0';
    InodoBlock.i_perm = 664;
    fseek(fileDisk,SBlock.s_inode_start,SEEK_SET);
    fwrite(&InodoBlock,sizeof(Tabla_Inodos),1,fileDisk);
    //Bloque para la carpeta root

    //Escribimos en la caprta actual
    strcpy(Block.b_content[0].b_name,".");
    Block.b_content[0].b_inodo=0;

    //Copiamos a la carpeta padre
    strcpy(Block.b_content[1].b_name,"..");
    Block.b_content[1].b_inodo=0;

    
    strcpy(Block.b_content[2].b_name,"users.txt");
    Block.b_content[2].b_inodo=1;

    strcpy(Block.b_content[3].b_name,".");
    Block.b_content[3].b_inodo=-1;
    fseek(fileDisk,SBlock.s_block_start,SEEK_SET);
    fwrite(&Block,sizeof(Bloque_Carpetas),1,fileDisk);
    //Llenamos parametros para los inodos de los usuarios
    InodoBlock.i_uid = 1;
    InodoBlock.i_gid = 1;
    InodoBlock.i_size = 27;
    InodoBlock.i_atime = time(nullptr);
    InodoBlock.i_ctime = time(nullptr);
    InodoBlock.i_mtime = time(nullptr);
    InodoBlock.i_block[0] = 1;
    for(int i = 1; i < 15;i++){
        InodoBlock.i_block[i] = -1;
    }
    InodoBlock.i_type = '1';
    InodoBlock.i_perm = 755;
    fseek(fileDisk,SBlock.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
    fwrite(&InodoBlock,sizeof(Tabla_Inodos),1,fileDisk);
    //Bloque para los archivos de los usuarios
    Bloque_Archivos archivo;
    memset(archivo.b_content,0,sizeof(archivo.b_content));
    strcpy(archivo.b_content,"1,G,root\n1,U,root,root,123\n");
    fseek(fileDisk,SBlock.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas)),SEEK_SET);
    fwrite(&archivo,sizeof(Bloque_Archivos),1,fileDisk);

    cout << "\033[96mFormateo de tipo EXT3 realizada con éxito\033[0m" << endl;
    fclose(fileDisk);//Cerramos el disco
}
void ComandoMKFS(string id,string type,int fs){
    string idtmp;
    bool flag=false;
    for(int i=0;i < arreglonodos.size();i++){
        string vd="VD";
        string letra="";
        letra = toupper(arreglonodos[i].letra);
        string numero = to_string(arreglonodos[i].numero);
        idtmp = vd+letra+numero;
        if(idtmp==id){ 
            int contador = FindPrimariaYExtendida(arreglonodos[i].path,arreglonodos[i].name);
                if(contador != -1){
                    MBR mbr;
                    FILE *fileDisk = fopen(arreglonodos[i].path.c_str(),"rb+");
                    fread(&mbr,sizeof(MBR),1,fileDisk);
                    int inicio = mbr.mbr_partition[contador].part_start;
                    int tamano = mbr.mbr_partition[contador].part_size;
                    if(fs == 2){
                        FormatEXT2(inicio,tamano,arreglonodos[i].path);
                        Mkfsids.push_back(id);
                    }
                    else if(fs==3){
                        FormatEXT3(inicio,tamano,arreglonodos[i].path);
                        Mkfsids.push_back(id);
                    }
                    fclose(fileDisk);
                }else{
                    contador = FindLogic(arreglonodos[i].path,arreglonodos[i].name);
                }            
            flag = true;
            break;    
        }
    }
    if(!flag){
        cout<<"\033[91m Error: No se encontro la partición montada con ese id!!!\n Las particiones montadas son: \033[0m"<<endl;
    }
}

/*
Funciones y metodos para el manejo del login y todas sus validaciones.
*/


int FindGrupo(string grupo){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");

    char arreglochar[400] = "\0";
    SuperBloque SB;
    Tabla_Inodos Inodo;

    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    //Leemos el Inodo del archivo users.txt
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)), SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);

    for(int i = 0; i < 15; i++){
        if(Inodo.i_block[i] != -1){
            Bloque_Archivos archivo;
            fseek(FileDisk,SB.s_block_start,SEEK_SET);
            for(int j = 0; j <= Inodo.i_block[i]; j++){
                fread(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
            }
            strcat(arreglochar,archivo.b_content);
        }
    }

    fclose(FileDisk);

    char *finalcadena;
    char *llave = strtok_r(arreglochar,"\n",&finalcadena);
    while(llave != nullptr){
        char id[2];
        char tipo[2];
        char group[12];
        char *Finalllave;
        char *llavei = strtok_r(llave,",",&Finalllave);
        strcpy(id,llavei);
        if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
            llavei = strtok_r(nullptr,",",&Finalllave);
            strcpy(tipo,llavei);
            if(strcmp(tipo,"G") == 0){
                strcpy(group,Finalllave);
                if(strcmp(group,grupo.c_str()) == 0)
                    return atoi(id);
            }
        }
        llave = strtok_r(nullptr,"\n",&finalcadena);
    }

    return -1;
}

int VerificarParametros(string user,string password,string direccion){
    FILE *FileDisk = fopen(direccion.c_str(),"rb+");

    char arreglochar[400] = "\0";
    SuperBloque SB;
    Tabla_Inodos Inodo;

    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    //Leemos el Inodo del archivo users.txt
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);

    for(int i = 0; i < 15; i++){
        if(Inodo.i_block[i] != -1){
            Bloque_Archivos archivo;
            fseek(FileDisk,SB.s_block_start,SEEK_SET);
            for(int j = 0; j <= Inodo.i_block[i]; j++){
                fread(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
            }
            strcat(arreglochar,archivo.b_content);
        }
    }

    fclose(FileDisk);

    char *finalcadena;
    char *llave = strtok_r(arreglochar,"\n",&finalcadena);
    while(llave != nullptr){
        char id[2];
        char tipo[2];
        string group;
        char user_[12];
        char password_[12];
        char *Finalllave;
        char *llavei = strtok_r(llave,",",&Finalllave);
        strcpy(id,llavei);
        if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
            llavei=strtok_r(nullptr,",",&Finalllave);
            strcpy(tipo,llavei);
            if(strcmp(tipo,"U") == 0){
                llavei = strtok_r(nullptr,",",&Finalllave);
                group = llavei;
                llavei = strtok_r(nullptr,",",&Finalllave);
                strcpy(user_,llavei);
                llavei = strtok_r(nullptr,",",&Finalllave);
                strcpy(password_,llavei);
                if(strcmp(user_,user.c_str()) == 0){
                    if(strcmp(password_,password.c_str()) == 0){
                        SesionActual.direccion = direccion;
                        SesionActual.IDU = atoi(id);
                        SesionActual.IDG = FindGrupo(group);
                        return 1;
                    }else{
                        return 2;
                    }
                        
                }
            }
        }
        llave = strtok_r(nullptr,"\n",&finalcadena);
    }

    return 0;
}

char ObtenerFit(string path,string nombre){
    FILE *FileDisk;
    if((FileDisk = fopen(path.c_str(),"rb+"))){
        int numeroExtendida = -1;
        MBR mbr;
        fseek(FileDisk,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,FileDisk);
        for(int i = 0; i < 4; i++){
            if(mbr.mbr_partition[i].part_type == 'E'){
                numeroExtendida = i;
                break;
            }
        }
        if(numeroExtendida != -1){
            EBR ebr;
            fseek(FileDisk, mbr.mbr_partition[numeroExtendida].part_start,SEEK_SET);
            while(fread(&ebr,sizeof(EBR),1,FileDisk)!=0 && (ftell(FileDisk) < mbr.mbr_partition[numeroExtendida].part_start + mbr.mbr_partition[numeroExtendida].part_size)){
                if(strcmp(ebr.part_name, nombre.c_str()) == 0){
                    return ebr.part_fit;
                }
            }
        }
        fclose(FileDisk);
    }
    return -1;
}
int VerificarLogin(string path,string nombre,string user,string password){
    int contador = FindPrimariaYExtendida(path,nombre);
    if(contador != -1){
        MBR mbr;
        SuperBloque SB;
        Tabla_Inodos Inodo;
        FILE *FileDisk = fopen(path.c_str(),"rb+");
        fread(&mbr,sizeof(MBR),1,FileDisk);
        fseek(FileDisk,mbr.mbr_partition[contador].part_start,SEEK_SET);
        fread(&SB,sizeof(SuperBloque),1,FileDisk);
        fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
        fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
        fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
        Inodo.i_atime = time(nullptr);
        fwrite(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
        fclose(FileDisk);
        SesionActual.PosicionInicialSB = mbr.mbr_partition[contador].part_start;
        SesionActual.fit = mbr.mbr_partition[contador].part_fit;
        SesionActual.PosicionInicialJournal = mbr.mbr_partition[contador].part_start + static_cast<int>(sizeof(SuperBloque));
        SesionActual.FS = SB.s_filesystem_type;
        return VerificarParametros(user,password, path);
    }else{
        contador = FindLogic(path, nombre);
        if(contador != -1){
            SuperBloque SB;
            Tabla_Inodos Inodo;
            FILE *FileDisk = fopen(path.c_str(),"rb+");
            fseek(FileDisk,contador + static_cast<int>(sizeof(EBR)),SEEK_SET);
            fread(&SB,sizeof(SuperBloque),1,FileDisk);
            fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
            fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
            fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
            Inodo.i_atime = time(nullptr);
            fwrite(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
            fclose(FileDisk);
            SesionActual.PosicionInicialSB = contador + static_cast<int>(sizeof(EBR));
            SesionActual.fit = ObtenerFit(path,nombre);
            return VerificarParametros(user,password,path);
        }
    }
    return 0;
}

void login(string usuario,string contrasena,string id){
    string idtmp;
    bool flag=false;
    bool mkfstrue=false;
    for(int i=0;i < arreglonodos.size();i++){
        string vd="VD";
        string letra="";
        letra = toupper(arreglonodos[i].letra);
        string numero = to_string(arreglonodos[i].numero);
        idtmp = vd+letra+numero;
        for(int i=0;i<Mkfsids.size();i++){
            if(Mkfsids[i]==idtmp){
                mkfstrue=true;
            }
        }
        if(idtmp==id){
            flag=true;
            if(!SesionActiva){
                if(mkfstrue){
                int retornoVerificacion = VerificarLogin(arreglonodos[i].path,arreglonodos[i].name,usuario,contrasena);
                if(retornoVerificacion == 1){
                    SesionActiva = true;
                    cout << "\033[96m Sesion iniciada con exito\033[0m" << endl;
                }else if(retornoVerificacion == 2)
                    cout << "\033[91m Error: Contrasena incorrecta\033[0m" << endl;
                else if(retornoVerificacion == 0)
                    cout << "\033[91m Error: Usuario no encontrado \033[0m" << endl;   
                break;    
            }else{
                cout << "\033[91m Error:Se debe formatear la particion con formato EXT2 o EXT3 antes de intentar iniciar sesion\033[0m" << endl;
            }             
        }else{
            cout<<"\033[91m Error: Se a encontrado una seccion activa porfavor haga un LOGOUT y vuelva a intentar loguearse \033[0m"<<endl;
        }
        }
    }
    if(!flag){
        cout<<"\033[91m Error: No se encontro la partición montada con ese id!!!\n Las particiones montadas son: \033[0m"<<endl;
    }
}

void logout(){
    if(SesionActiva){
        SesionActiva = false;
        SesionActual.IDU=-1;
        SesionActual.direccion="";
        SesionActual.PosicionInicialSB=-1;
        cout<<"\033[96mSesion cerrada con exito!!\033[0m"<<endl;
    }else{
        cout<<"\033[91m No hay sesion activa la cual se pueda cerrar!!\033[0m"<<endl;
    }
}
string EliminarComillas(string cadena){
    char replacement[] = "";
    string result = cadena.replace(0,1, replacement);
    result = result.replace(cadena.size()-1,1,replacement);
    return result;
}
void MKGRP(string name){
    string nombreGrupo = EliminarComillasYreemplazarEspacios(name); 
    if(SesionActiva){
        if(SesionActual.IDU == 1 && SesionActual.IDG == 1){//Usuario root
            if(nombreGrupo.length() <= 10){
                int grupo = FindGrupo(nombreGrupo);
                if(grupo == -1){
                    int idGrp = ObtenerIDG();
                    string nuevoGrupo = to_string(idGrp)+",G,"+nombreGrupo+"\n";
                    AddUserstxt(nuevoGrupo);
                    cout << "\033[96m El grupo se creo exitosamente :D \033[0m "<< endl;
                    //Guardamos el registro en el journal si es un EXT3
                    if(SesionActual.FS ==3){
                        char aux[64];
                        char comando[10];
                        char Contenido[2];
                        strcpy(aux,nuevoGrupo.c_str());
                        strcpy(comando,"mkgrp");
                        memset(Contenido,0,2);
                        SaveJournaling(comando,0  ,0,aux,Contenido);
                    }
                }else
                    cout << "\033[91m Error: Este grupo ya Exist, cambiele nombre porfavor \033[0m" << endl;
            }else
                cout << "\033[91m Error: El nombre es muy largo, maximo 10 caracteres \033[0m " << endl;
        }else
            cout << " \033[91m Error: No esta logeado con el usuario ROOT, accion no posible!! \033[0m " << endl;
    }else
        cout << "\033[91m Error: Sesion no iniciada \033[0m " << endl;
}
int ObtenerIDG(){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");

    char arreglochar[400] = "\0";
    int Identificadortmp = -1;
    SuperBloque SB;
    Tabla_Inodos Inodo;
    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)), SEEK_SET);//Posicionamos para el Inodo
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);//Leemos el Inodo.. del txt

    for(int i = 0; i < 15; i++){
        if(Inodo.i_block[i] != -1){
            Bloque_Archivos file;
            fseek(FileDisk,SB.s_block_start,SEEK_SET);
            for(int j = 0; j <= Inodo.i_block[i]; j++){
                fread(&file,sizeof(Bloque_Archivos),1,FileDisk);
            }
            strcat(arreglochar,file.b_content);
        }
    }

    fclose(FileDisk);

    char *FinCadena;
    char *llave = strtok_r(arreglochar,"\n",&FinCadena);
    while(llave != nullptr){
        char id[2];
        char tipo[2];
        char *finllave;
        char *token2 = strtok_r(llave,",",&finllave);
        strcpy(id,token2);
        if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
            token2 = strtok_r(nullptr,",",&finllave);
            strcpy(tipo,token2);
            if(strcmp(tipo,"G") == 0){
                Identificadortmp = atoi(id);
            }

        }
        llave = strtok_r(nullptr,"\n",&FinCadena);
    }
    return ++Identificadortmp;
}
void SaveJournaling(char* comando,int tipo,int permisos,char *nombre,char *content){
    SuperBloque SB;
    Journaling registroEXT3;
    strcpy(registroEXT3.Journal_tipo_operacion,comando);
    registroEXT3.Journal_tipo = tipo;
    strcpy(registroEXT3.Journal_nombre,nombre);
    strcpy(registroEXT3.Journal_contenido,content);
    registroEXT3.Journal_fecha = time(nullptr);
    registroEXT3.Journal_propietario = SesionActual.IDU;
    registroEXT3.Journal_permisos = permisos;
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");
    Journaling registroEXT3tmp;
    bool fin = false;
    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    int JournalingIni = SesionActual.PosicionInicialSB + static_cast<int>(sizeof(SuperBloque));
    int JournalingFin = SB.s_bm_inode_start;
    fseek(FileDisk,JournalingIni,SEEK_SET);
    while((ftell(FileDisk) < JournalingFin) && !fin){
        fread(&registroEXT3tmp,sizeof(Journaling),1,FileDisk);
        if(registroEXT3tmp.Journal_tipo != 0 && registroEXT3tmp.Journal_tipo != 1)
            fin = true;
    }
    fseek(FileDisk,ftell(FileDisk)- static_cast<int>(sizeof(Journaling)),SEEK_SET);
    fwrite(&registroEXT3,sizeof(Journaling),1,FileDisk);
    fclose(FileDisk);
}
void AddUserstxt(string datos){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(), "rb+");

    SuperBloque SB;
    Tabla_Inodos Inodo;
    Bloque_Archivos ArchivoBloque;
    int NumBloque = 0;

    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)), SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);

    for(int i = 0; i < 12; i++){
        if(Inodo.i_block[i] != -1)
            NumBloque = Inodo.i_block[i];
    }

    fseek(FileDisk,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*NumBloque,SEEK_SET);
    fread(&ArchivoBloque,sizeof(Bloque_Archivos),1,FileDisk);
    int EspacioUsado = static_cast<int>(strlen(ArchivoBloque.b_content));
    int EspacioLibre = 63 - EspacioUsado;

    if(datos.length() <= EspacioLibre){
        strcat(ArchivoBloque.b_content,datos.c_str());
        fseek(FileDisk,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*NumBloque,SEEK_SET);
        fwrite(&ArchivoBloque,sizeof(Bloque_Archivos),1,FileDisk);
        fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
        fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
        Inodo.i_size = Inodo.i_size + datos.length();
        Inodo.i_mtime = time(nullptr);
        fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
        fwrite(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
    }else{
        string tmp = "";
        string tmp1 = "";
        int i = 0;

        for(i = 0; i < EspacioLibre; i++)
            tmp += datos.at(i);

        for(; i < datos.length(); i++)
            tmp1  += datos.at(i);

        /*
        En el primer bloque guardamos lo que le quepa
        */
        strcat(ArchivoBloque.b_content,tmp.c_str());
        fseek(FileDisk,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*NumBloque,SEEK_SET);
        fwrite(&ArchivoBloque,sizeof(Bloque_Archivos),1,FileDisk);
        Bloque_Archivos auxArchivo;
        strcpy(auxArchivo.b_content,tmp1.c_str());
        int bit = FindBit(FileDisk,'B',SesionActual.fit);
        /*
        Escribimos en el bitmap y en la tabla de bloques el bloque
        */

        fseek(FileDisk,SB.s_bm_block_start + bit,SEEK_SET);
        fputc('2',FileDisk);
        fseek(FileDisk,SB.s_block_start + (static_cast<int>(sizeof(Bloque_Archivos))*bit),SEEK_SET);
        fwrite(&auxArchivo,sizeof(Bloque_Archivos),1,FileDisk);

        /*
        Escribimos el Inodo que se cambio
        */
        fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
        fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
        Inodo.i_size = Inodo.i_size + datos.length();
        Inodo.i_mtime = time(nullptr);
        Inodo.i_block[NumBloque] = bit;
        fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
        fwrite(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
        /*
        Escribimos los bloques que quedan libres y seteamos el primero disponible ahra
        */
        SB.s_first_blo = SB.s_first_blo + 1;
        SB.s_free_blocks_count = SB.s_free_blocks_count - 1;
        fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
        fwrite(&SB,sizeof(SuperBloque),1,FileDisk);
    }
    fclose(FileDisk);
}
int FindBit(FILE *FileDisk, char tipo, char fit){
    SuperBloque SB;
    int BitmapInicio = 0;
    char Bitaux = '0';
    int BitDisponible = -1;
    int BitmapTamanio = 0;

    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);

    if(tipo == 'I'){
        BitmapTamanio = SB.s_inodes_count;
        BitmapInicio = SB.s_bm_inode_start;
    }else if(tipo == 'B'){
        BitmapTamanio = SB.s_blocks_count;
        BitmapInicio = SB.s_bm_block_start;
    }

    if(fit == 'F'){     //Primer ajuste
        for(int i = 0; i < BitmapTamanio; i++){
            fseek(FileDisk,BitmapInicio + i,SEEK_SET);
            Bitaux = static_cast<char>(fgetc(FileDisk));
            if(Bitaux == '0'){
                BitDisponible = i;
                return BitDisponible;
            }
        }

        if(BitDisponible == -1)
            return -1;

    }else if(fit == 'B'){   //Mejor ajuste
        int libres = 0;
        int auxLibres = -1;

        /*
        Hacemos el recorrido inicial
        */
        for(int i = 0; i < BitmapTamanio; i++){
            fseek(FileDisk,BitmapInicio + i,SEEK_SET);
            Bitaux = static_cast<char>(fgetc(FileDisk));
            if(Bitaux == '0'){
                libres++;
                if(i+1 == BitmapTamanio){
                    if(auxLibres == -1 || auxLibres == 0)
                        auxLibres = libres;
                    else{
                        if(auxLibres > libres)
                            auxLibres = libres;
                    }
                    libres = 0;
                }
            }else if(Bitaux == '1'){
                if(auxLibres == -1 || auxLibres == 0)
                    auxLibres = libres;
                else{
                    if(auxLibres > libres)
                        auxLibres = libres;
                }
                libres = 0;
            }
        }

        for(int i = 0; i < BitmapTamanio; i++){
            fseek(FileDisk,BitmapInicio + i, SEEK_SET);
            Bitaux = static_cast<char>(fgetc(FileDisk));
            if(Bitaux == '0'){
                libres++;
                if(i+1 == BitmapTamanio)
                    return ((i+1)-libres);
            }else if(Bitaux == '1'){
                if(auxLibres == libres)
                    return ((i+1) - libres);
                libres = 0;
            }
        }

        return -1;

    }else if(fit == 'W'){//Peor ajuste
        int libres = 0;
        int auxLibres = -1;

        for (int i = 0; i < BitmapTamanio; i++) {
            fseek(FileDisk,BitmapInicio + i, SEEK_SET);
            Bitaux = static_cast<char>(fgetc(FileDisk));
            if(Bitaux == '0'){
                libres++;
                if(i+1 == BitmapTamanio){
                    if(auxLibres == -1 || auxLibres == 0)
                        auxLibres = libres;
                    else {
                        if(auxLibres < libres)
                            auxLibres = libres;
                    }
                    libres = 0;
                }
            }else if(Bitaux == '1'){
                if(auxLibres == -1 || auxLibres == 0)
                    auxLibres = libres;
                else{
                    if(auxLibres < libres)
                        auxLibres = libres;
                }
                libres = 0;
            }
        }

        for (int i = 0; i < BitmapTamanio; i++) {
            fseek(FileDisk,BitmapInicio + i, SEEK_SET);
            Bitaux = static_cast<char>(fgetc(FileDisk));
            if(Bitaux == '0'){
                libres++;
                if(i+1 == BitmapTamanio)
                    return ((i+1) - libres);
            }else if(Bitaux == '1'){
                if(auxLibres == libres)
                    return ((i+1) - libres);
                libres = 0;
            }
        }

        return -1;
    }

    return 0;
}
void RMGRP(string name){
    string nombreGrupo = EliminarComillasYreemplazarEspacios(name);
    if(SesionActiva){
        if(SesionActual.IDU == 1 && SesionActual.IDG == 1){//Usuario root
            int grupo = FindGrupo(nombreGrupo);
            if(grupo != -1){
                REMOVERGRUPO(nombreGrupo);
            }else
                cout << "\033[91m Error: Grupo inexistente \033[0m " << endl;
        }else
           cout << "\033[91m Error: Comando solo peude ser ejecutado por el usuario ROOT \033[0m " << endl;
    }else
        cout << "\033[91m Error: Sesion no iniciada \033[0m " << endl;
}
void REMOVERGRUPO(string name){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");

    SuperBloque SB;
    Tabla_Inodos Inodo;
    Bloque_Archivos archivo;

    int columna = 1;
    char actual;
    int posicion = 0;
    int BloqueContador = 0;
    int id = -1;
    char tipo = '\0';
    string grupo = "";
    string palabra = "";
    bool tmp = false;


    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    //Nos posicionamos en el Inodo del archivo users.txt
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);

    for (int i = 0; i < 12; i++) {
        if(Inodo.i_block[i] != -1){
            fseek(FileDisk,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*Inodo.i_block[i],SEEK_SET);
            fread(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
            for(int j = 0; j < 63; j++){
                actual = archivo.b_content[j];
                if(actual=='\n'){
                    if(tipo == 'G'){
                        grupo = palabra;
                        if(strcmp(grupo.c_str(),name.c_str()) == 0){
                            fseek(FileDisk,SB.s_block_start+static_cast<int>(sizeof(Bloque_Archivos))*BloqueContador,SEEK_SET);
                            fread(&archivo,sizeof(Bloque_Carpetas),1,FileDisk);
                            archivo.b_content[posicion] = '0';
                            fseek(FileDisk,SB.s_block_start+static_cast<int>(sizeof(Bloque_Archivos))*BloqueContador,SEEK_SET);
                            fwrite(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
                            cout << "\033[96m El grupo se elimino exitosamente!! :D \033[0m" << endl;
                            tmp = true;
                            break;
                        }
                    }
                    columna = 1;
                    palabra = "";
                }else if(actual != ','){
                    palabra += actual;
                    columna++;
                }else if(actual == ','){
                    if(columna == 2){
                        id = atoi(palabra.c_str());
                        posicion = j-1;
                        BloqueContador = Inodo.i_block[i];
                    }
                    else if(columna == 4)
                        tipo = palabra[0];
                    columna++;
                    palabra = "";
                }
            }
            if(tmp)
                break;
        }
    }

    fclose(FileDisk);
}

void MKUSR(string userx, string pass, string groupx){
    string user = EliminarComillasYreemplazarEspacios(userx);
    string group = EliminarComillasYreemplazarEspacios(groupx);
    if(user.length() <= 10){
        if(pass.length() <= 10){
            if(group.length() <= 10){
                if(SesionActiva){
                    if(SesionActual.IDU == 1 && SesionActual.IDG == 1){//Usuario root
                        if(FindGrupo(group) != -1){
                            if(!buscarUsuario(user)){
                                int id = getID_usr();
                                string datos = to_string(id) + ",U,"+group+","+user+","+pass+"\n";
                                AddUserstxt(datos);
                                cout << "\033[96m El usuario se creo exitosamente!! :D \033[0m"  << endl;
                                //Guardamos el registro en el journal si es un sistema EXT3
                                if(SesionActual.FS ==3){
                                    char aux[64];
                                    char comando[10];
                                    char content[2];
                                    strcpy(aux,datos.c_str());
                                    strcpy(comando,"mkusr");
                                    memset(content,0,2);
                                    SaveJournaling(comando,0,0,aux,content);
                                }
                            }else
                                cout << "\033[91m Error: Este usuario ya existe \033[0m " <<endl;
                        }else
                            cout << "\033[91m Error: No se encuentra el grupo de este usuario \033[0m  " << endl;
                    }else
                        cout << "\033[91m Error: Este comando solamente es para el usuario ROOT lo siento :c \033[0m " << endl;
                }else
                    cout << "\033[91m Error: Debe iniciar sesión para poder ejecutar el comando \033[0m " << endl;
            }else
                cout << "\033[91m Error: El nombre del grupo excede los 10 caracteres permitidos ;( \033[0m " << endl;
        }else
            cout << "\033[91m Error: La contraseña excede los 10 caracters permitidos!! \033[0m " << endl;
    }else
        cout << "\033[91m Error: El nombre del usuario excede los 10 caracteres permitidos :c !\033[0m " << endl;
}
bool buscarUsuario(string name){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");

    char cadena[400] = "\0";
    SuperBloque SB;
    Tabla_Inodos Inodo;

    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    //Nos posicionamos en el Inodo del archivo users.txt
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)), SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);

    for(int i = 0; i < 15; i++){
        if(Inodo.i_block[i] != -1){
            Bloque_Archivos archivo;
            fseek(FileDisk,SB.s_block_start,SEEK_SET);
            for(int j = 0; j <= Inodo.i_block[i]; j++){
                fread(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
            }
            strcat(cadena,archivo.b_content);
        }
    }

    fclose(FileDisk);

    char *end_str;
    char *llave = strtok_r(cadena,"\n",&end_str);
    while(llave != nullptr){
        char id[2];
        char tipo[2];
        char user[12];
        char *end_token;
        char *token2 = strtok_r(llave,",",&end_token);
        strcpy(id,token2);
        if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
            token2 = strtok_r(nullptr,",",&end_token);
            strcpy(tipo,token2);
            if(strcmp(tipo,"U") == 0){
                token2 = strtok_r(nullptr,",",&end_token);
                token2 = strtok_r(nullptr,",",&end_token);
                strcpy(user,token2);

                if(strcmp(user,name.c_str()) == 0)
                    return true;
            }
        }
        llave = strtok_r(nullptr,"\n",&end_str);
    }

    return false;
}
int getID_usr(){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");

    char cadena[400] = "\0";
    int res = 0;
    SuperBloque SB;
    Tabla_Inodos Inodo;

    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    //Nos posicionamos en el Inodo del archivo users.txt
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)), SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);

    for(int i = 0; i < 15; i++){
        if(Inodo.i_block[i] != -1){
            Bloque_Archivos archivo;
            fseek(FileDisk,SB.s_block_start,SEEK_SET);
            for(int j = 0; j <= Inodo.i_block[i]; j++){
                fread(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
            }
            strcat(cadena,archivo.b_content);
        }
    }

    fclose(FileDisk);

    char *end_str;
    char *llave = strtok_r(cadena,"\n",&end_str);
    while(llave != nullptr){
        char id[2];
        char tipo[2];
        char *end_token;
        char *token2 = strtok_r(llave,",",&end_token);
        strcpy(id,token2);
        if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
            token2 = strtok_r(nullptr,",",&end_token);
            strcpy(tipo,token2);
            if(strcmp(tipo,"U") == 0)
                res++;
        }
        llave = strtok_r(nullptr,"\n",&end_str);
    }
    return ++res;
}


void RMUSR(string userName){
    if(SesionActiva){
        if(SesionActual.IDU == 1 && SesionActual.IDG == 1){
            if(buscarUsuario(userName)){
                eliminarUsuario(userName);
            }else
                cout << "\033[91m Error: El usuario no se encontro, no existe.\033[0m " << endl;
        }else
           cout << "\033[91m Error: Lo sentimos pero este comando solo esta permitido para el usuario ROOT \033[0m " << endl;
    }else
        cout << "\033[91m Error: Debe iniciar sesión para ejecutar comando (user ROOT) !!\033[0m " << endl;
}
void eliminarUsuario(string name){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");

    SuperBloque SB;
    Tabla_Inodos Inodo;
    Bloque_Archivos archivo;

    int col = 1;
    char actual;
    string palabra = "";
    int posicion = 0;
    int numBloque = 0;
    int id = -1;
    char tipo = '\0';
    string grupo = "";
    string usuario = "";
    bool flag = false;

    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos)),SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);

    for (int i = 0; i < 12; i++) {
        if(Inodo.i_block[i] != -1){
            fseek(FileDisk,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*Inodo.i_block[i],SEEK_SET);
            fread(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
            for(int j = 0; j < 63; j++){
                actual = archivo.b_content[j];
                if(actual=='\n'){
                    if(tipo == 'U'){
                        if(strcmp(usuario.c_str(),name.c_str()) == 0){
                            fseek(FileDisk,SB.s_block_start+static_cast<int>(sizeof(Bloque_Archivos))*numBloque,SEEK_SET);
                            fread(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
                            archivo.b_content[posicion] = '0';
                            fseek(FileDisk,SB.s_block_start+static_cast<int>(sizeof(Bloque_Archivos))*numBloque,SEEK_SET);
                            fwrite(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
                            cout << "\033[96m Se eliminó el usuario con éxito :D \033[0m " << endl;
                            flag = true;
                            break;
                        }
                        usuario = "";
                        grupo = "";
                    }
                    col = 1;
                    palabra = "";
                }else if(actual != ','){
                    palabra += actual;
                    col++;
                }else if(actual == ','){
                    if(col == 2){
                        id = atoi(palabra.c_str());
                        posicion = j-1;
                        numBloque = Inodo.i_block[i];
                    }
                    else if(col == 4)
                        tipo = palabra[0];
                    else if(grupo == "")
                        grupo = palabra;
                    else if(usuario == "")
                        usuario = palabra;
                    col++;
                    palabra = "";
                }
            }
            if(flag)
                break;
        }
    }

    fclose(FileDisk);
}

void cambiarPermisosRecursivo(FILE* archivo, int n, int permisos){
    SuperBloque SB;
    Tabla_Inodos Inodo;
    Bloque_Carpetas carpeta;
    char b ='0';

    fseek(archivo,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,archivo);
    fseek(archivo,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*n,SEEK_SET);
    fread(&Inodo,sizeof(Tabla_Inodos),1,archivo);
    Inodo.i_perm = permisos;
    fseek(archivo,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*n,SEEK_SET);
    fwrite(&Inodo,sizeof(Tabla_Inodos),1,archivo);

    for(int i = 0; i < 15; i++){
        if(Inodo.i_block[i] != -1){
            fseek(archivo,SB.s_bm_block_start + Inodo.i_block[i],SEEK_SET);
            b = static_cast<char>(fgetc(archivo));
            if(b == '1'){
                fseek(archivo,SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*Inodo.i_block[i],SEEK_SET);
                fread(&carpeta,sizeof(Bloque_Carpetas),1,archivo);
                for(int j = 0; j < 4; j++){
                    if(carpeta.b_content[j].b_inodo != -1){
                        if(strcmp(carpeta.b_content[j].b_name,".")!=0 &&  strcmp(carpeta.b_content[j].b_name,"..")!=0)
                            cambiarPermisosRecursivo(archivo,carpeta.b_content[j].b_inodo,permisos);
                    }
                }
            }
        }
    }
}

void CHMOD(string pathg, string ugo,bool flagR){
    if(SesionActiva){

        int user =stoi(to_string(ugo.at(0)).c_str()) - '0';//Obtener el valor numero de un char
        int grupo = stoi(to_string(ugo.at(0)).c_str()) - '0';
        int otros = stoi(to_string(ugo.at(0)).c_str()) - '0';
        if((user >= 0 && user <= 7) && (grupo >= 0 && grupo <= 7) && (otros >= 0 && otros <= 7)){
            char auxPath[500];
            strcpy(auxPath,pathg.c_str());
            FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");
            SuperBloque SB;
            Tabla_Inodos Inodo;
            int Exist = buscarCarpetaArchivo(FileDisk,auxPath);
            fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
            fread(&SB,sizeof(SuperBloque),1,FileDisk);
            fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*Exist,SEEK_SET);
            fread(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
            if(Exist != -1){
                if((SesionActual.IDU ==1 && SesionActual.IDG == 1) || SesionActual.IDU == Inodo.i_uid){
                    if(flagR)
                        cambiarPermisosRecursivo(FileDisk,Exist,stoi(ugo));
                    else{
                        Inodo.i_perm = stoi(ugo);
                        fseek(FileDisk,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*Exist,SEEK_SET);
                        fwrite(&Inodo,sizeof(Tabla_Inodos),1,FileDisk);
                    }
                    cout << "\033[96m Permisos cambiados exitosamente!! \033[0m " << endl;
                }else
                    cout << "\033[91m Error: Para cambiar los permisos debes ser propietario de la capeta o el archivo, sino estar logeado con el usuario root \033[0m " << endl;
            }else
                cout << "\033[91m Error: La ruta no existe. \033[0m " << endl;
            fclose(FileDisk);
        }else
            cout << "\033[91m Error: Algún dígito del UGO esta fuera de rango, solo esta permitido del 0 hasta el 7 \033[0m "<< endl;
    }else
        cout << "\033[91m Error: Debe iniciar sesión para ejecutar este comando...\033[0m " << endl;
}

int buscarCarpetaArchivo(FILE *archivo, char* path){
    SuperBloque SB;
    Tabla_Inodos Inodo;
    Bloque_Carpetas carpeta;
    BloqueDeApuntadores Punter;

    vector<string> lista;
    char *llave = strtok(path,"/");
    int cont = 0;
    int numInodo = 0;

    while(llave != nullptr){
        lista.push_back(llave);
        cont++;
        llave = strtok(nullptr,"/");
    }

    fseek(archivo,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,archivo);
    //Posicionamos para el bit donde inicia el inodo
    numInodo = SB.s_inode_start; 

    for (int cont2 = 0; cont2 < cont; cont2++) {
        fseek(archivo,numInodo,SEEK_SET);
        fread(&Inodo,sizeof(Tabla_Inodos),1,archivo);
        int siguiente = 0;
        for(int i = 0; i < 15; i++){
            if(Inodo.i_block[i] != -1){
                //Apuntador directo
                int byteBloque = byteInodoBloque(archivo,Inodo.i_block[i],'2');
                fseek(archivo,byteBloque,SEEK_SET);
                if(i < 12){
                    fread(&carpeta,sizeof(Bloque_Carpetas),1,archivo);
                    for (int j = 0; j < 4; j++) {
                        if((cont2 == cont - 1) && (strcasecmp(carpeta.b_content[j].b_name,lista.at(cont2).c_str()) == 0)){//Haceos match con la carpeta
                            return carpeta.b_content[j].b_inodo;
                        }else if((cont2 != cont - 1) && (strcasecmp(carpeta.b_content[j].b_name,lista.at(cont2).c_str()) == 0)){
                            numInodo = byteInodoBloque(archivo,carpeta.b_content[j].b_inodo,'1');
                            siguiente = 1;
                            break;
                        }
                    }
                }else if(i == 12){
                    //Apuntador inidrecto
                    fread(&Punter,sizeof(BloqueDeApuntadores),1,archivo);
                    for(int j = 0; j < 16; j++){
                        if(Punter.b_pointers[j] != -1){
                            byteBloque = byteInodoBloque(archivo,Punter.b_pointers[j],'2');
                            fseek(archivo,byteBloque,SEEK_SET);
                            fread(&carpeta,sizeof(Bloque_Carpetas),1,archivo);
                            for (int k = 0; k < 4; k++) {
                                if((cont2 == cont - 1) && (strcasecmp(carpeta.b_content[k].b_name,lista.at(cont2).c_str()) == 0)){//Haceos match con la carpeta
                                    return carpeta.b_content[k].b_inodo;
                                }else if((cont2 != cont - 1) && (strcasecmp(carpeta.b_content[k].b_name,lista.at(cont2).c_str()) == 0)){
                                    numInodo = byteInodoBloque(archivo,carpeta.b_content[k].b_inodo,'1');
                                    siguiente = 1;
                                    break;
                                }
                            }
                            if(siguiente == 1)
                                break;
                        }
                    }
                }else if(i == 13){

                }else if(i == 14){

                }
                if(siguiente == 1)
                    break;
            }
        }
    }

    return -1;
}
int byteInodoBloque(FILE *archivo,int pos, char tipo){
    SuperBloque SB;
    fseek(archivo,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,archivo);
    if(tipo == '1'){
        return (SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*pos);
    }else if(tipo == '2')
        return (SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*pos);
    return 0;
}


void MKFILE(string ruta,string Cont,int tamanio,bool P,string name){
    /*
    cout<<"ruta:"<<ruta<<endl;
    cout<<"cont:"<<Cont<<endl;
    cout<<"tamanio:"<<tamanio<<endl;
    cout<<"-P:"<<P<<endl;
    cout<<"name:"<<name<<endl;
    */
    if(name.length() <= 11){
                if(SesionActiva){
                    cout<<"entre al mfile sesion atvia"<<endl;
                    int ArchivoCreadoConExito = crearArchivo(ruta,P,tamanio,Cont);
                    //cout<<ArchivoCreadoConExito<<endl;
                    if(ArchivoCreadoConExito == 1){//Si el metodo nos devuelve que si se creo con exito
                        if(SesionActual.FS == 3){//Si el disco es EXT3
                            char tmp[500];
                            char comando[8];
                            char content[500];
                            strcpy(tmp,ruta.c_str());
                            strcpy(comando,"mkfile");
                            strcpy(content,Cont.c_str());
                            if(Cont.length() != 0)
                                SaveJournaling(comando,0,664,tmp,content);
                            else{
                                strcpy(content,to_string(tamanio).c_str());
                                SaveJournaling(comando,0,664,tmp,content);
                            }
                        }
                        cout << "\033[96m Archivo creado con exito!! \033[0m " << endl;
                    }else if(ArchivoCreadoConExito == 2)
                        cout << "\033[91m El usuario no tiene permisos de escritura \033[0m" << endl;
                    else if(ArchivoCreadoConExito == 3)
                        cout << "\033[91m El archivo no existe \033[0m" << endl;
                    else if(ArchivoCreadoConExito == 4)
                        cout << "\033[91m La ruta de directorios no existe y el comando -p no esta activado para crearlo\033[0m" << endl;
                }else
                    cout << "\033[91m Porfavor inicie sesion para ejecutar este comando \033[0m" << endl;
            }else
                cout << "\033[91m El nombre sobrepasa el limite de caracteres \033[0m" << endl;
                
}
int crearArchivo(string path, bool p, int size, string cont){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");

    SuperBloque SB;
    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);

    char RutaAuxiliar[500];
    char RutaAuxiliartmp[500];
    strcpy(RutaAuxiliar,path.c_str());
    strcpy(RutaAuxiliartmp,path.c_str());
    int existe = buscarCarpetaArchivo(FileDisk,RutaAuxiliar);
    strcpy(RutaAuxiliar,path.c_str());
    int retorno = -1;

    if(existe != -1)
        retorno = 0;
    else
        retorno = nuevoArchivo(FileDisk,SesionActual.fit,p,RutaAuxiliar,size,cont,0,RutaAuxiliartmp);

    fclose(FileDisk);
    return retorno;
}

int nuevoArchivo(FILE *stream, char fit, bool P, char *path, int size, string contenido, int index,char *auxPath){
    SuperBloque SB;
    Tabla_Inodos inodo,inodoNuevo;
    Bloque_Carpetas carpeta, carpetaNueva;
    BloqueDeApuntadores apuntadores;
    vector<string> lista;
    char Pathtmp[500];
    char directorio[500];
    char nombreCarpeta[80];
    string content = "";
    string contentSize = "0123456789";

    strcpy(Pathtmp,path);
    strcpy(directorio,dirname(Pathtmp));
    strcpy(Pathtmp,path);
    strcpy(nombreCarpeta,basename(Pathtmp));
    strcpy(Pathtmp,path);
    char *token = strtok(path,"/");
    int cont = 0;
    int numInodo = index;
    int finalSize = size;

    while(token != nullptr){
        cont++;
        lista.push_back(token);
        token = strtok(nullptr,"/");
    }

    if(contenido.length() != 0){
        FILE *archivoCont;
        if((archivoCont = fopen(contenido.c_str(),"r"))){
            fseek(archivoCont,0,SEEK_END);
            finalSize = static_cast<int>(ftell(archivoCont));
            fseek(archivoCont,0,SEEK_SET);
            for (int i = 0; i < finalSize; i++)
                content += static_cast<char>(fgetc(archivoCont));
        }else
            return 3;
    }

    fseek(stream,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,stream);

    if(cont == 1){
        int bloque = 0;
        int pointer = 0;
        int b_content = 0;
        int libre = buscarContentLibre(stream,numInodo,inodo,carpeta,apuntadores,b_content,bloque,pointer);

        if(libre == 1){
            bool permisos = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == SesionActual.IDU),(inodo.i_gid == SesionActual.IDG));
            if(permisos || (SesionActual.IDU == 1 && SesionActual.IDG == 1)){
                char buffer = '1';
                char buffer2 = '2';
                char buffer3 = '3';
                //Agregamos el archivo al bloque correspondiente
                int bitInodo = FindBit(stream,'I',fit);
                carpeta.b_content[b_content].b_inodo = bitInodo;
                strcpy(carpeta.b_content[b_content].b_name,nombreCarpeta);
                fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*inodo.i_block[bloque],SEEK_SET);
                fwrite(&carpeta,sizeof(Bloque_Carpetas),1,stream);
                //Creamos el nuevo inodo archivo
                inodoNuevo = crearInodo(0,'1',664);
                fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*bitInodo,SEEK_SET);
                fwrite(&inodoNuevo,sizeof(Tabla_Inodos),1,stream);
                //Registramos el inodo en el bitmap
                fseek(stream,SB.s_bm_inode_start + bitInodo,SEEK_SET);
                fwrite(&buffer,sizeof(char),1,stream);
                //Si viene el parametro -size/-cont
                if(finalSize != 0){
                    double n = static_cast<double>(finalSize)/static_cast<double>(63);
                    int numBloques = static_cast<int>(ceil(n));
                    int caracteres = finalSize;
                    size_t charNum = 0;
                    size_t contChar = 0;
                    numInodo = buscarCarpetaArchivo(stream,auxPath);
                    for (int i = 0; i < numBloques; i++) {
                        Bloque_Archivos archivo;
                        memset(archivo.b_content,0,sizeof(archivo.b_content));
                        if(i == 12){
                            int bitBloqueA = FindBit(stream,'B',fit);//Apuntador
                            //Guardamos el bloque en el inodo archivo
                            fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                            fread(&inodo,sizeof(Tabla_Inodos),1,stream);
                            inodo.i_block[i] = bitBloqueA;
                            fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                            fwrite(&inodo,sizeof(Tabla_Inodos),1,stream);
                            //Registramos el bloque apuntador en el bitmap
                            fseek(stream,SB.s_bm_block_start + bitBloqueA,SEEK_SET);
                            fwrite(&buffer3,sizeof(char),1,stream);
                            //Creamos el bloque de apuntadores como es simple solo se debe crear uno
                            int bitBloque = FindBit(stream,'B',fit);//Primer bloque archivo
                            apuntadores.b_pointers[0] = bitBloque;
                            for(int i = 1; i < 16; i++)
                                apuntadores.b_pointers[i] = -1;
                            fseek(stream,SB.s_block_start + static_cast<int>(sizeof(BloqueDeApuntadores))*bitBloqueA,SEEK_SET);
                            fwrite(&apuntadores,sizeof(BloqueDeApuntadores),1,stream);
                            //Empezamos con la creacion de bloques archivos
                            //Registramos el primer bloque en el bitmap
                            fseek(stream,SB.s_bm_block_start + bitBloque,SEEK_SET);
                            fwrite(&buffer2,sizeof(char),1,stream);
                            if(caracteres > 63){
                                for(int j = 0; j < 63; j++){
                                    if(content.length() != 0){//-cont
                                        archivo.b_content[j] = content[contChar];
                                        contChar++;
                                    }else{//-size
                                        archivo.b_content[j] = contentSize[charNum];
                                        charNum++;
                                        if(charNum == 10)
                                            charNum = 0;
                                    }
                                }
                                //Guardamos el bloque
                                fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*bitBloque,SEEK_SET);
                                fwrite(&archivo,sizeof(Bloque_Archivos),1,stream);
                                caracteres -= 63;
                            }else{
                                for (int j = 0; j < caracteres; j++) {
                                    if(content.length() != 0){
                                        archivo.b_content[j] = content[contChar];
                                        contChar++;
                                    }else{
                                        archivo.b_content[j] = contentSize[charNum];
                                        charNum++;
                                        if(charNum == 10)
                                            charNum = 0;
                                    }
                                }
                                //Guardamos el bloque
                                fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*bitBloque,SEEK_SET);
                                fwrite(&archivo,sizeof(Bloque_Archivos),1,stream);
                            }
                        }else if(i > 12 && i < 28){
                            int libre = 0;
                            fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                            fread(&inodo,sizeof(Tabla_Inodos),1,stream);
                            fseek(stream,SB.s_block_start + static_cast<int>(sizeof(BloqueDeApuntadores))*inodo.i_block[12],SEEK_SET);
                            fread(&apuntadores,sizeof(BloqueDeApuntadores),1,stream);
                            for (int a = 0; a < 16; a++) {
                                if(apuntadores.b_pointers[a] == -1){
                                    libre = a;
                                    break;
                                }
                            }
                            int bitBloque = FindBit(stream,'B',fit);
                            apuntadores.b_pointers[libre] = bitBloque;
                            fseek(stream,SB.s_block_start + static_cast<int>(sizeof(BloqueDeApuntadores))*inodo.i_block[12],SEEK_SET);
                            fwrite(&apuntadores,sizeof(BloqueDeApuntadores),1,stream);
                            //Registramos el bloque en el bitmap
                            fseek(stream,SB.s_bm_block_start + bitBloque,SEEK_SET);
                            fwrite(&buffer2,sizeof(char),1,stream);
                            if(caracteres > 63){
                                for(int j = 0; j < 63; j++){
                                    if(content.length() != 0){//-cont
                                        archivo.b_content[j] = content[contChar];
                                        contChar++;
                                    }else{//-size
                                        archivo.b_content[j] = contentSize[charNum];
                                        charNum++;
                                        if(charNum == 10)
                                            charNum = 0;
                                    }
                                }
                                //Guardamos el bloque
                                fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*bitBloque,SEEK_SET);
                                fwrite(&archivo,sizeof(Bloque_Archivos),1,stream);
                                caracteres -= 63;
                            }else{
                                for (int j = 0; j < caracteres; j++) {
                                    if(content.length() != 0){
                                        archivo.b_content[j] = content[contChar];
                                        contChar++;
                                    }else{
                                        archivo.b_content[j] = contentSize[charNum];
                                        charNum++;
                                        if(charNum == 10)
                                            charNum = 0;
                                    }
                                }
                                //Guardamos el bloque
                                fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*bitBloque,SEEK_SET);
                                fwrite(&archivo,sizeof(Bloque_Archivos),1,stream);
                            }
                        }else if(i == 29){//Apuntador doble

                        }else{
                            int bitBloque = FindBit(stream,'B',fit);
                            //Registramos el bloque en el bitmap
                            fseek(stream,SB.s_bm_block_start + bitBloque,SEEK_SET);
                            fwrite(&buffer2,sizeof(char),1,stream);
                            if(caracteres > 63){
                                for(int j = 0; j < 63; j++){
                                    if(content.length() != 0){//-cont
                                        archivo.b_content[j] = content[contChar];
                                        contChar++;
                                    }else{//-size
                                        archivo.b_content[j] = contentSize[charNum];
                                        charNum++;
                                        if(charNum == 10)
                                            charNum = 0;
                                    }
                                }
                                //Guardamos el bloque en el respectivo inodo archivo
                                fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                                fread(&inodo,sizeof(Tabla_Inodos),1,stream);
                                inodo.i_block[i] = bitBloque;
                                inodo.i_size = finalSize;
                                fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                                fwrite(&inodo,sizeof(Tabla_Inodos),1,stream);
                                //Guardamos el bloque
                                fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*bitBloque,SEEK_SET);
                                fwrite(&archivo,sizeof(Bloque_Archivos),1,stream);
                                caracteres -= 63;
                            }else{
                                for (int j = 0; j < caracteres; j++) {
                                    if(content.length() != 0){
                                        archivo.b_content[j] = content[contChar];
                                        contChar++;
                                    }else{
                                        archivo.b_content[j] = contentSize[charNum];
                                        charNum++;
                                        if(charNum == 10)
                                            charNum = 0;
                                    }
                                }
                                //Guardamos el bloque en el respectivo inodo archivo
                                fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                                fread(&inodo,sizeof(Tabla_Inodos),1,stream);
                                inodo.i_block[i] = bitBloque;
                                inodo.i_size = finalSize;
                                fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                                fwrite(&inodo,sizeof(Tabla_Inodos),1,stream);
                                //Guardamos el bloque
                                fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*bitBloque,SEEK_SET);
                                fwrite(&archivo,sizeof(Bloque_Archivos),1,stream);
                            }
                        }
                    }
                    //Modificamos el SB bloque
                    SB.s_free_blocks_count = SB.s_free_blocks_count - numBloques;
                    SB.s_free_inodes_count = SB.s_free_inodes_count - 1;
                    SB.s_first_ino = SB.s_first_ino + 1;
                    SB.s_first_blo = SB.s_first_blo + numBloques;
                    fseek(stream,SesionActual.PosicionInicialSB,SEEK_SET);
                    fwrite(&SB,sizeof(SuperBloque),1,stream);
                    return 1;
                }
                SB.s_free_inodes_count = SB.s_free_inodes_count - 1;
                SB.s_first_ino = SB.s_first_ino + 1;
                fseek(stream,SesionActual.PosicionInicialSB,SEEK_SET);
                fwrite(&SB,sizeof(SuperBloque),1,stream);
                return 1;
            }else
                return 2;
        }else{//Todos los bloques estan llenos
            fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
            fread(&inodo,sizeof(Tabla_Inodos),1,stream);
            for (int i = 0; i < 15; i++) {
                if(inodo.i_block[i] == -1){
                    bloque = i;
                    break;
                }
            }
            if(bloque == 12){//Apuntador simple indirecto
                bool permissions = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == SesionActual.IDU),(inodo.i_gid == SesionActual.IDG));
                if(permissions || (SesionActual.IDU == 1 && SesionActual.IDG == 1) ){

                }else
                    return 2;
            }else if(bloque == 13){//Apuntador doble indirecto

            }else if(bloque == 14){//Apuntador triple indirecto

            }else{//Apuntadores directos
                bool permisos = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == SesionActual.IDU),(inodo.i_gid == SesionActual.IDG));
                if(permisos || (SesionActual.IDU == 1 && SesionActual.IDG == 1)){
                    char buffer = '1';
                    char buffer2 = '2';
                    char buffer3 = '3';
                    int bitBloque = FindBit(stream,'B',fit);
                    //Guardamos el bloque en el inodo
                    inodo.i_block[bloque] = bitBloque;
                    fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                    fwrite(&inodo,sizeof(Tabla_Inodos),1,stream);
                    //Creamos el nuevo bloque carpeta
                    int bitInodo = FindBit(stream,'I',fit);
                    carpetaNueva.b_content[0].b_inodo = bitInodo;
                    carpetaNueva.b_content[1].b_inodo = -1;
                    carpetaNueva.b_content[2].b_inodo = -1;
                    carpetaNueva.b_content[3].b_inodo = -1;
                    strcpy(carpetaNueva.b_content[0].b_name,nombreCarpeta);
                    strcpy(carpetaNueva.b_content[1].b_name,"");
                    strcpy(carpetaNueva.b_content[2].b_name,"");
                    strcpy(carpetaNueva.b_content[3].b_name,"");
                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*bitBloque,SEEK_SET);
                    fwrite(&carpetaNueva,sizeof(Bloque_Carpetas),1,stream);
                    //Registramos el bloque en el bitmap
                    fseek(stream,SB.s_bm_block_start + bitBloque,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Creamos el nuevo inodo
                    inodoNuevo = crearInodo(0,'1',664);
                    fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*bitInodo,SEEK_SET);
                    fwrite(&inodoNuevo,sizeof(Tabla_Inodos),1,stream);
                    fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*bitInodo,SEEK_SET);
                    fwrite(&inodoNuevo,sizeof(Tabla_Inodos),1,stream);
                    //Registramos el inodo en el bitmap
                    fseek(stream,SB.s_bm_inode_start + bitInodo,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Si viene el parametro -size/-cont
                    if(finalSize != 0){
                        double n = static_cast<double>(finalSize)/static_cast<double>(63);
                        int numBloques = static_cast<int>(ceil(n));
                        int caracteres = finalSize;
                        size_t charNum = 0;
                        size_t contChar = 0;
                        numInodo = buscarCarpetaArchivo(stream,auxPath);
                        for (int i = 0; i < numBloques; i++) {
                            Bloque_Archivos archivo;
                            memset(archivo.b_content,0,sizeof(archivo.b_content));
                            if(i == 12){
                                int bitBloqueA = FindBit(stream,'B',fit);//Apuntador
                                //Guardamos el bloque en el inodo archivo
                                fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                                fread(&inodo,sizeof(Tabla_Inodos),1,stream);
                                inodo.i_block[i] = bitBloqueA;
                                fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                                fwrite(&inodo,sizeof(Tabla_Inodos),1,stream);
                                //Registramos el bloque apuntador en el bitmap
                                fseek(stream,SB.s_bm_block_start + bitBloqueA,SEEK_SET);
                                fwrite(&buffer3,sizeof(char),1,stream);
                                //Creamos el bloque de apuntadores como es simple solo se debe crear uno
                                int bitBloque = FindBit(stream,'B',fit);//Primer bloque archivo
                                apuntadores.b_pointers[0] = bitBloque;
                                for(int i = 1; i < 16; i++)
                                    apuntadores.b_pointers[i] = -1;
                                fseek(stream,SB.s_block_start + static_cast<int>(sizeof(BloqueDeApuntadores))*bitBloqueA,SEEK_SET);
                                fwrite(&apuntadores,sizeof(BloqueDeApuntadores),1,stream);
                                //Empezamos con la creacion de bloques archivos
                                //Registramos el primer bloque en el bitmap
                                fseek(stream,SB.s_bm_block_start + bitBloque,SEEK_SET);
                                fwrite(&buffer2,sizeof(char),1,stream);
                                if(caracteres > 63){
                                    for(int j = 0; j < 63; j++){
                                        if(content.length() != 0){//-cont
                                            archivo.b_content[j] = content[contChar];
                                            contChar++;
                                        }else{//-size
                                            archivo.b_content[j] = contentSize[charNum];
                                            charNum++;
                                            if(charNum == 10)
                                                charNum = 0;
                                        }
                                    }
                                    //Guardamos el bloque
                                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*bitBloque,SEEK_SET);
                                    fwrite(&archivo,sizeof(Bloque_Archivos),1,stream);
                                    caracteres -= 63;
                                }else{
                                    for (int j = 0; j < caracteres; j++) {
                                        if(content.length() != 0){
                                            archivo.b_content[j] = content[contChar];
                                            contChar++;
                                        }else{
                                            archivo.b_content[j] = contentSize[charNum];
                                            charNum++;
                                            if(charNum == 10)
                                                charNum = 0;
                                        }
                                    }
                                    //Guardamos el bloque
                                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*bitBloque,SEEK_SET);
                                    fwrite(&archivo,sizeof(Bloque_Archivos),1,stream);
                                }
                            }else if(i > 12 && i < 28){
                                int libre = 0;
                                fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                                fread(&inodo,sizeof(Tabla_Inodos),1,stream);
                                fseek(stream,SB.s_block_start + static_cast<int>(sizeof(BloqueDeApuntadores))*inodo.i_block[12],SEEK_SET);
                                fread(&apuntadores,sizeof(BloqueDeApuntadores),1,stream);
                                for (int a = 0; a < 16; a++) {
                                    if(apuntadores.b_pointers[a] == -1){
                                        libre = a;
                                        break;
                                    }
                                }
                                int bitBloque = FindBit(stream,'B',fit);
                                apuntadores.b_pointers[libre] = bitBloque;
                                fseek(stream,SB.s_block_start + static_cast<int>(sizeof(BloqueDeApuntadores))*inodo.i_block[12],SEEK_SET);
                                fwrite(&apuntadores,sizeof(BloqueDeApuntadores),1,stream);
                                //Registramos el bloque en el bitmap
                                fseek(stream,SB.s_bm_block_start + bitBloque,SEEK_SET);
                                fwrite(&buffer2,sizeof(char),1,stream);
                                if(caracteres > 63){
                                    for(int j = 0; j < 63; j++){
                                        if(content.length() != 0){//-cont
                                            archivo.b_content[j] = content[contChar];
                                            contChar++;
                                        }else{//-size
                                            archivo.b_content[j] = contentSize[charNum];
                                            charNum++;
                                            if(charNum == 10)
                                                charNum = 0;
                                        }
                                    }
                                    //Guardamos el bloque
                                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*bitBloque,SEEK_SET);
                                    fwrite(&archivo,sizeof(Bloque_Archivos),1,stream);
                                    caracteres -= 63;
                                }else{
                                    for (int j = 0; j < caracteres; j++) {
                                        if(content.length() != 0){
                                            archivo.b_content[j] = content[contChar];
                                            contChar++;
                                        }else{
                                            archivo.b_content[j] = contentSize[charNum];
                                            charNum++;
                                            if(charNum == 10)
                                                charNum = 0;
                                        }
                                    }
                                    //Guardamos el bloque
                                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*bitBloque,SEEK_SET);
                                    fwrite(&archivo,sizeof(Bloque_Archivos),1,stream);
                                }

                            }else if(i == 29){//Apuntador doble

                            }else{//Apuntadores simples
                                int bitBloque = FindBit(stream,'B',fit);
                                //Registramos el bloque en el bitmap
                                fseek(stream,SB.s_bm_block_start + bitBloque,SEEK_SET);
                                fwrite(&buffer2,sizeof(char),1,stream);
                                if(caracteres > 63){
                                    for(int j = 0; j < 63; j++){
                                        if(content.length() != 0){//-cont
                                            archivo.b_content[j] = content[contChar];
                                            contChar++;
                                        }else{//-size
                                            archivo.b_content[j] = contentSize[charNum];
                                            charNum++;
                                            if(charNum == 10)
                                                charNum = 0;
                                        }
                                    }
                                    //Guardamos el bloque en el respectivo inodo archivo
                                    fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                                    fread(&inodo,sizeof(Tabla_Inodos),1,stream);
                                    inodo.i_block[i] = bitBloque;
                                    fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                                    fwrite(&inodo,sizeof(Tabla_Inodos),1,stream);
                                    //Guardamos el bloque
                                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*bitBloque,SEEK_SET);
                                    fwrite(&archivo,sizeof(Bloque_Archivos),1,stream);
                                    caracteres -= 63;
                                }else{
                                    for (int j = 0; j < caracteres; j++) {
                                        if(content.length() != 0){
                                            archivo.b_content[j] = content[contChar];
                                            contChar++;
                                        }else{
                                            archivo.b_content[j] = contentSize[charNum];
                                            charNum++;
                                            if(charNum == 10)
                                                charNum = 0;
                                        }
                                    }
                                    //Guardamos el bloque en el respectivo inodo archivo
                                    fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                                    fread(&inodo,sizeof(Tabla_Inodos),1,stream);
                                    inodo.i_block[i] = bitBloque;
                                    fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                                    fwrite(&inodo,sizeof(Tabla_Inodos),1,stream);
                                    //Guardamos el bloque
                                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Archivos))*bitBloque,SEEK_SET);
                                    fwrite(&archivo,sizeof(Bloque_Archivos),1,stream);
                                }
                            }
                        }
                        //Modificamos el SB bloque
                        SB.s_free_blocks_count = SB.s_free_blocks_count - numBloques;
                        SB.s_free_inodes_count = SB.s_free_inodes_count - 1;
                        SB.s_first_ino = SB.s_first_ino + 1;
                        SB.s_first_blo = SB.s_first_blo + numBloques;
                        fseek(stream,SesionActual.PosicionInicialSB,SEEK_SET);
                        fwrite(&SB,sizeof(SuperBloque),1,stream);
                        return 1;
                    }
                    SB.s_free_inodes_count = SB.s_free_inodes_count - 1;
                    SB.s_first_ino = SB.s_first_ino + 1;
                    fseek(stream,SesionActual.PosicionInicialSB,SEEK_SET);
                    fwrite(&SB,sizeof(SuperBloque),1,stream);
                    return 1;
                }else
                    return 2;
            }
        }
    }else{//Directorio
        int existe = buscarCarpetaArchivo(stream,directorio);
        if(existe == -1){
            if(P){
                int index = 0;
                string tmp = "";
                //Crear posibles carpetas inexistentes
                for (int i = 0; i < cont; i++) {
                    if(i == cont -1){
                        char dir[100] = "/";
                        strcat(dir,nombreCarpeta);
                        return nuevoArchivo(stream,fit,false,dir,size,contenido,index,auxPath);
                    }else{
                        tmp += "/"+lista.at(i);
                        char dir[500];
                        char auxDir[500];
                        strcpy(dir,tmp.c_str());
                        strcpy(auxDir,tmp.c_str());
                        int carpeta = buscarCarpetaArchivo(stream,dir);
                        if(carpeta == -1){
                            nuevaCarpeta(stream,fit,false,auxDir,index);
                            strcpy(auxDir,tmp.c_str());
                            index = buscarCarpetaArchivo(stream,auxDir);
                        }else
                            index = carpeta;
                    }
                }
            }else
                return 4;
        }else{//Crear el archivo en el directorio
            char dir[100] = "/";
            strcat(dir,nombreCarpeta);
            return nuevoArchivo(stream,fit,false,dir,size,contenido,existe,auxPath);
        }
    }

    return 0;
}

int buscarContentLibre(FILE* stream,int numInodo,Tabla_Inodos &inodo,Bloque_Carpetas &carpeta, BloqueDeApuntadores &apuntadores,int &content,int &bloque,int &pointer){
    int BanderaLibre = 0;
    SuperBloque SB;
    fseek(stream,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,stream);
    fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
    fread(&inodo,sizeof(Tabla_Inodos),1,stream);
    /*
    Buscamos en el bloque de la carpeta un espacio libre
    */
    for(int i = 0; i < 15; i++){
        if(inodo.i_block[i] != -1){
            if(i == 12){
                //Apuntador indirecto simple
                fseek(stream,SB.s_block_start + static_cast<int>(sizeof(BloqueDeApuntadores))*inodo.i_block[i],SEEK_SET);
                fread(&apuntadores,sizeof(BloqueDeApuntadores),1,stream);
                for(int j = 0; j < 16; j++){
                    if(apuntadores.b_pointers[j] != -1){
                        fseek(stream,SB.s_block_start + static_cast<int>(sizeof(BloqueDeApuntadores))*apuntadores.b_pointers[j],SEEK_SET);
                        fread(&carpeta,sizeof(Bloque_Carpetas),1,stream);
                        for(int k = 0; k < 4; k++){
                            if(carpeta.b_content[k].b_inodo == -1){
                                BanderaLibre = 1;
                                bloque = i;
                                pointer = j;
                                content = k;
                                break;
                            }
                        }
                    }
                    if(BanderaLibre == 1)
                        break;
                }
            }else if(i == 13){
                //Apuntador indirecto doble(sin hacer)

            }else if(i == 14){
                //Apuntador indirecto tripe(sin hacer)

            }else{
                //Apuntador directo
                fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*inodo.i_block[i],SEEK_SET);
                fread(&carpeta,sizeof(Bloque_Carpetas),1,stream);
                for(int j = 0; j < 4; j++){
                    if(carpeta.b_content[j].b_inodo == -1){
                        BanderaLibre = 1;
                        bloque = i;
                        content = j;
                        break;
                    }
                }
            }
        }
        if(BanderaLibre == 1)
            break;
    }

    return BanderaLibre;
}

/*
Nos devuelve verdadero o falso si se tienen los permisos para escribir 
*/
bool permisosDeEscritura(int permisos, bool flagUser, bool flagGroup){
    string stringpermisos = to_string(permisos);
    char propietario = stringpermisos[0];
    char grupo = stringpermisos[1];
    char otros = stringpermisos[2];

    //Retornamos los permisos comparando cada uno de los caracteres de UGO
    if((propietario == '2' || propietario == '3' || propietario == '6' || propietario || '7') && flagUser)
        return true;
    else if((grupo == '2' || grupo == '3' || grupo == '6' || grupo == '7') && flagGroup)
        return true;
    else if(otros == '2' || otros == '3' || otros == '6' || otros == '7')
        return true;

    return false;
}
/*
Nos devuelve verdadero o falso si se tienen los permisos para leer 
*/
bool permisosDeLectura(int permisos, bool flagUser, bool flagGroup){
    string stringpermisos = to_string(permisos);
    int propietario = stringpermisos[0] - '0';
    int grupo = stringpermisos[1] - '0';
    int otros = stringpermisos[2] - '0';

    //ESTABLECEMOS LOS PERMISOS ANALIZANDO CARACTER POR CARACTER QUE VIENEN EN EL UGO
    if((propietario >= 3) && flagUser)
        return true;
    else if((grupo >= 3) && flagGroup)
        return true;
    else if(otros >= 3)
        return true;

    return false;
}


//Metodo que crea un Inodo, recibe el tamanio el tipo y los diferentes permisos.
Tabla_Inodos crearInodo(int size,char type,int perm){
    Tabla_Inodos inodo ;
    inodo.i_uid = SesionActual.IDU;
    inodo.i_gid = SesionActual.IDG;
    inodo.i_size = size;
    inodo.i_atime = time(nullptr);
    inodo.i_ctime = time(nullptr);
    inodo.i_mtime = time(nullptr);
    for(int i = 0; i < 15; i++)
        inodo.i_block[i] = -1;
    inodo.i_type = type;
    inodo.i_perm = perm;
    return inodo;
}

/*
Carpeta que retorna 3 valores 1 exitoso, 2 no tiene permisos , 3 flag -p no escrito y no existen los archivos.
*/
int nuevaCarpeta(FILE *stream, char fit, bool P, char *path, int index){
    //Declaramos los objetos de las estructuras a utilizar
    SuperBloque SB;
    Tabla_Inodos inodo,inodoNuevo;
    Bloque_Carpetas carpeta, carpetaNueva, carpetaAux;
    BloqueDeApuntadores apuntadores;
    vector<string> lista;

    /*
    Declaramos 3 arreglos char para copiar el pat, obtener el nombre del directorio en la ruta completa, y tambien el nombre base de la carpeta que se creara
    nueva....
    */
    char Pathtmp[500];
    char directorio[500];
    char nombreCarpeta[80];
    strcpy(Pathtmp,path);
    strcpy(directorio,dirname(Pathtmp));
    strcpy(Pathtmp,path);
    strcpy(nombreCarpeta,basename(Pathtmp));

    /*
    Spliteamos la ruta por los nombres de cada directorio  y los pusheamos por medio de un while en una lista...
    */
    char *token = strtok(path,"/");
    int cont = 0;
    int numInodo = index;
    int response = 0;

    while(token != nullptr){
        cont++;
        lista.push_back(token);
        token = strtok(nullptr,"/");
    }

    /*
    Posicionamos al inicio del SUPERBLOQUE y lo leemos todo.
    */
    fseek(stream,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,stream);

    //Si la direccion es de tamanio 1 osea que solamente viene como que una carpeta ej /user o /home
    if(cont == 1){
        int Contenido = 0;
        int bloque = 0;
        int pointer = 0;
        int libre = buscarContentLibre(stream,numInodo,inodo,carpeta,apuntadores,Contenido,bloque,pointer);
        if(libre == 1){
            if(bloque == 12){//REgistros de bloques directos
                bool permissions = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == SesionActual.IDU),(inodo.i_gid == SesionActual.IDG));
                if(permissions || (SesionActual.IDU == 1 && SesionActual.IDG == 1) ){
                    char buffer = '1';
                    int bitInodo = FindBit(stream,'I',fit);
                    /*
                    Buscamos el bit disponible para agregar la carpeta al espacio libre que tenga el bloque
                    
                    */
                    carpeta.b_content[Contenido].b_inodo = bitInodo;
                    strcpy(carpeta.b_content[Contenido].b_name,nombreCarpeta);
                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*apuntadores.b_pointers[pointer],SEEK_SET);
                    fwrite(&carpeta,sizeof(Bloque_Carpetas),1,stream);
                    /*
                    Creamos el nuevo inodo de la carpeta...
                    */
                    inodoNuevo = crearInodo(0,'0',664);
                    int bitBloque = FindBit(stream,'B',fit);
                    inodoNuevo.i_block[0] = bitBloque;
                    fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*bitInodo,SEEK_SET);
                    fwrite(&inodoNuevo,sizeof(Tabla_Inodos),1,stream);
                    /*
                    Posicionamos en el inodo el bitmap y guardamos
                    */
                    fseek(stream,SB.s_bm_inode_start + bitInodo,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    /*
                    Creamos el nuevo bloque paara la carpeta! 
                    */
                    carpetaNueva = crearBloqueCarpeta();
                    carpetaNueva.b_content[0].b_inodo = bitInodo;
                    carpetaNueva.b_content[1].b_inodo = numInodo;
                    strcpy(carpetaNueva.b_content[0].b_name,".");
                    strcpy(carpetaNueva.b_content[1].b_name,"..");
                    /*
                    Posicionamos y escribimos en el bloque de carpetas
                    */
                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*bitBloque,SEEK_SET);
                    fwrite(&carpetaNueva,sizeof(Bloque_Carpetas),1,stream);
                    /*
                    Posicionamos y escribimos en el bitmap del bloque.
                    */
                    fseek(stream,SB.s_bm_block_start + bitBloque,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Sobreescribimos el SB bloque
                    /*
                    Ahora le volvemos a dar los valores nuevos al superbloque
                    */
                    SB.s_free_inodes_count = SB.s_free_inodes_count - 1;
                    SB.s_free_blocks_count = SB.s_free_blocks_count - 1;
                    SB.s_first_ino = SB.s_first_ino + 1;
                    SB.s_first_blo = SB.s_first_blo + 1;
                    /*
                    Posicionamos y escribimos el superbloque en el disco
                    */
                    fseek(stream,SesionActual.PosicionInicialSB,SEEK_SET);
                    fwrite(&SB,sizeof(SuperBloque),1,stream);
                    return 1;
                }else
                    return 2;
            }else if(bloque == 13){
                //Puntero indirecto doble :( no lo hice
            }else if(bloque == 14){
                //Puntero doble indirecto
            }else{//Sino seran apuntadores directos
                bool permisos = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == SesionActual.IDU),(inodo.i_gid == SesionActual.IDG));
                if(permisos || (SesionActual.IDU == 1 && SesionActual.IDG == 1) ){
                    char buffer = '1';
                    int bitInodo = FindBit(stream,'I',fit);
                    //Agregamos la carpeta al espacio libre en el bloque
                    carpeta.b_content[Contenido].b_inodo = bitInodo;
                    strcpy(carpeta.b_content[Contenido].b_name,nombreCarpeta);
                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*inodo.i_block[bloque],SEEK_SET);
                    fwrite(&carpeta,sizeof(Bloque_Carpetas),1,stream);
                    //Creamos el nuevo inodo
                    inodoNuevo = crearInodo(0,'0',664);
                    int bitBloque = FindBit(stream,'B',fit);
                    inodoNuevo.i_block[0] = bitBloque;
                    fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*bitInodo,SEEK_SET);
                    fwrite(&inodoNuevo,sizeof(Tabla_Inodos),1,stream);
                    //Creamos el nuevo bloque carpeta
                    carpetaNueva = crearBloqueCarpeta();
                    carpetaNueva.b_content[0].b_inodo = bitInodo;
                    carpetaNueva.b_content[1].b_inodo = numInodo;
                    strcpy(carpetaNueva.b_content[0].b_name,".");
                    strcpy(carpetaNueva.b_content[1].b_name,"..");
                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*bitBloque,SEEK_SET);
                    fwrite(&carpetaNueva,sizeof(Bloque_Carpetas),1,stream);
                    //Guardamos los bits en los bitmaps
                    fseek(stream,SB.s_bm_inode_start + bitInodo,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    fseek(stream,SB.s_bm_block_start + bitBloque,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Sobreescribimos el SB bloque
                    SB.s_free_inodes_count = SB.s_free_inodes_count - 1;
                    SB.s_free_blocks_count = SB.s_free_blocks_count - 1;
                    SB.s_first_ino = SB.s_first_ino + 1;
                    SB.s_first_blo = SB.s_first_blo + 1;
                    fseek(stream,SesionActual.PosicionInicialSB,SEEK_SET);
                    fwrite(&SB,sizeof(SuperBloque),1,stream);
                    return 1;
                }else
                    return 2;
            }
        }else if(libre == 0){//Todos bloques estan llenos
            bool flag = false;//Primera vez
            pointer = -1;
            fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
            fread(&inodo,sizeof(Tabla_Inodos),1,stream);
            for (int i = 0; i < 15; i++) {
                if(i == 12){//Apuntador indirecto simple
                    if(inodo.i_block[i] == -1){//Primera vez
                        bloque = 12;
                        flag = true;
                        break;
                    }else{
                        fseek(stream,SB.s_block_start + static_cast<int>(sizeof(BloqueDeApuntadores))*inodo.i_block[12],SEEK_SET);
                        fread(&apuntadores,sizeof(BloqueDeApuntadores),1,stream);
                        for(int j = 0; j < 16; j++){
                            if(apuntadores.b_pointers[j] == -1){
                                bloque = 12;
                                pointer = j;
                                break;
                            }
                        }
                    }
                    if(flag || pointer!= -1)
                        break;
                }else if(i == 13){

                }else if(i == 14){

                }else{
                    if(inodo.i_block[i] == -1){
                        bloque = i;
                        break;
                    }
                }
            }

            if(bloque == 12 && flag){//Apuntador indirecto simple
                bool permissions = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == SesionActual.IDU),(inodo.i_gid == SesionActual.IDG));
                if(permissions || (SesionActual.IDU == 1 && SesionActual.IDG == 1) ){
                    char buffer = '1';
                    char buffer3 = '3';
                    //Guardamos el bloque en el inodo
                    int bitBloque = FindBit(stream,'B',fit);//Apuntador
                    inodo.i_block[bloque] = bitBloque;
                    fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                    fwrite(&inodo,sizeof(Tabla_Inodos),1,stream);
                    //Escribimos el bit del bloque apuntador en el bitmap
                    fseek(stream,SB.s_bm_block_start + bitBloque,SEEK_SET);
                    fwrite(&buffer3,sizeof(char),1,stream);
                    //Creamos el bloque de apuntadores como es simple solo se debe crear uno
                    int bitBloqueCarpeta = FindBit(stream,'B',fit);//Carpeta
                    apuntadores.b_pointers[0] = bitBloqueCarpeta;
                    for(int i = 1; i < 16; i++)
                        apuntadores.b_pointers[i] = -1;
                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(BloqueDeApuntadores))*bitBloque,SEEK_SET);
                    fwrite(&apuntadores,sizeof(BloqueDeApuntadores),1,stream);
                    //Creamos la carpeta del apuntador
                    int bitInodo = FindBit(stream,'I',fit);
                    carpetaAux = crearBloqueCarpeta();
                    carpetaAux.b_content[0].b_inodo = bitInodo;
                    strcpy(carpetaAux.b_content[0].b_name,nombreCarpeta);
                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*bitBloqueCarpeta,SEEK_SET);
                    fwrite(&carpetaAux,sizeof(Bloque_Carpetas),1,stream);
                    //Escribimos el bit del bloque carpeta en el bitmap
                    fseek(stream,SB.s_bm_block_start + bitBloqueCarpeta,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Creamos el nuevo inodo carpeta
                    inodoNuevo = crearInodo(0,'0',664);
                    bitBloque = FindBit(stream,'B',fit);//Carpeta
                    inodoNuevo.i_block[0] = bitBloque;
                    fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*bitInodo,SEEK_SET);
                    fwrite(&inodoNuevo,sizeof(Tabla_Inodos),1,stream);
                    //Escribimos el bit del inodo en el bitmap
                    fseek(stream,SB.s_bm_inode_start + bitInodo,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Creamos el nuevo bloque carpeta
                    carpetaNueva = crearBloqueCarpeta();
                    carpetaNueva.b_content[0].b_inodo = bitInodo;
                    carpetaNueva.b_content[1].b_inodo = numInodo;
                    strcpy(carpetaNueva.b_content[0].b_name,".");
                    strcpy(carpetaNueva.b_content[1].b_name,"..");
                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*bitBloque,SEEK_SET);
                    fwrite(&carpetaNueva,sizeof(Bloque_Carpetas),1,stream);
                    //Guardamos el bit en el bitmap de bloques
                    fseek(stream,SB.s_bm_block_start + bitBloque,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Sobreescribimos el SB bloque
                    SB.s_free_inodes_count = SB.s_free_inodes_count - 1;
                    SB.s_free_blocks_count = SB.s_free_blocks_count - 3;
                    SB.s_first_ino = SB.s_first_ino + 1;
                    SB.s_first_blo = SB.s_first_blo + 3;
                    fseek(stream,SesionActual.PosicionInicialSB,SEEK_SET);
                    fwrite(&SB,sizeof(SuperBloque),1,stream);
                    return 1;
                }else
                    return 2;
            }else if(bloque == 12 && !flag){
                char buffer = '1';
                //Escribir el numero de bloque en el bloque de apuntadores
                int bitBloque = FindBit(stream,'B',fit);
                apuntadores.b_pointers[pointer] = bitBloque;
                fseek(stream,SB.s_block_start + static_cast<int>(sizeof(BloqueDeApuntadores))*inodo.i_block[12],SEEK_SET);
                fwrite(&apuntadores,sizeof(BloqueDeApuntadores),1,stream);
                //Creamos el bloque auxiliar
                int bitInodo = FindBit(stream,'I',fit);
                carpetaAux = crearBloqueCarpeta();
                carpetaAux.b_content[0].b_inodo = bitInodo;
                strcpy(carpetaAux.b_content[0].b_name,nombreCarpeta);
                fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*bitBloque,SEEK_SET);
                fwrite(&carpetaAux,sizeof(Bloque_Carpetas),1,stream);
                //Escribimos el bit del bloque carpeta en el bitmap
                fseek(stream,SB.s_bm_block_start + bitBloque,SEEK_SET);
                fwrite(&buffer,sizeof(char),1,stream);
                //Creamos el nuevo inodo carpeta
                inodoNuevo = crearInodo(0,'0',664);
                inodoNuevo.i_uid = SesionActual.IDU;
                inodoNuevo.i_gid = SesionActual.IDG;
                bitBloque = FindBit(stream,'B',fit);//Carpeta
                inodoNuevo.i_block[0] = bitBloque;
                fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*bitInodo,SEEK_SET);
                fwrite(&inodoNuevo,sizeof(Tabla_Inodos),1,stream);
                //Escribimos el bit del inodo en el bitmap
                fseek(stream,SB.s_bm_inode_start + bitInodo,SEEK_SET);
                fwrite(&buffer,sizeof(char),1,stream);
                //Creamos el nuevo bloque carpeta
                carpetaNueva = crearBloqueCarpeta();
                carpetaNueva.b_content[0].b_inodo = bitInodo;
                carpetaNueva.b_content[1].b_inodo = numInodo;
                strcpy(carpetaNueva.b_content[0].b_name,".");
                strcpy(carpetaNueva.b_content[1].b_name,"..");
                fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*bitBloque,SEEK_SET);
                fwrite(&carpetaNueva,sizeof(Bloque_Carpetas),1,stream);
                //Guardamos el bit en el bitmap de bloques
                fseek(stream,SB.s_bm_block_start + bitBloque,SEEK_SET);
                fwrite(&buffer,sizeof(char),1,stream);
                //Sobreescribimos el SB bloque
                SB.s_free_inodes_count = SB.s_free_inodes_count - 1;
                SB.s_free_blocks_count = SB.s_free_blocks_count - 2;
                SB.s_first_ino = SB.s_first_ino + 1;
                SB.s_first_blo = SB.s_first_blo + 2;
                fseek(stream,SesionActual.PosicionInicialSB,SEEK_SET);
                fwrite(&SB,sizeof(SuperBloque),1,stream);
                return 1;
            }
            else if(bloque == 13){//Apuntador indirecto doble

            }else if(bloque == 14){//Apuntador indirecto triple

            }else{//Apuntadores directos
                bool permissions = permisosDeEscritura(inodo.i_perm,(inodo.i_uid == SesionActual.IDU),(inodo.i_gid == SesionActual.IDG));
                if(permissions || (SesionActual.IDU == 1 && SesionActual.IDG == 1) ){
                    char buffer = '1';
                    int bitBloque = FindBit(stream,'B',fit);
                    inodo.i_block[bloque] = bitBloque;
                    //Sobreescribimos el inodo
                    fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*numInodo,SEEK_SET);
                    fwrite(&inodo,sizeof(Tabla_Inodos),1,stream);
                    //Bloque carpeta auxiliar
                    int bitInodo = FindBit(stream,'I',fit);
                    carpetaAux = crearBloqueCarpeta();
                    carpetaAux.b_content[0].b_inodo = bitInodo;
                    strcpy(carpetaAux.b_content[0].b_name,nombreCarpeta);
                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*bitBloque,SEEK_SET);
                    fwrite(&carpetaAux,sizeof(Bloque_Carpetas),1,stream);
                    //Escribimos el bit en el bitmap de blqoues
                    fseek(stream,SB.s_bm_block_start + bitBloque,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Creamos el nuevo inodo
                    inodoNuevo = crearInodo(0,'0',664);
                    bitBloque = FindBit(stream,'B',fit);
                    inodoNuevo.i_block[0] = bitBloque;
                    fseek(stream,SB.s_inode_start + static_cast<int>(sizeof(Tabla_Inodos))*bitInodo,SEEK_SET);
                    fwrite(&inodoNuevo,sizeof(Tabla_Inodos),1,stream);
                    //Escribimos el bit en el bitmap de inodos
                    fseek(stream,SB.s_bm_inode_start + bitInodo,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Creamos el nuevo bloque carpeta
                    carpetaNueva = crearBloqueCarpeta();
                    carpetaNueva.b_content[0].b_inodo = bitInodo;
                    carpetaNueva.b_content[1].b_inodo = numInodo;
                    strcpy(carpetaNueva.b_content[0].b_name,".");
                    strcpy(carpetaNueva.b_content[1].b_name,"..");
                    fseek(stream,SB.s_block_start + static_cast<int>(sizeof(Bloque_Carpetas))*bitBloque,SEEK_SET);
                    fwrite(&carpetaNueva,sizeof(Bloque_Carpetas),1,stream);
                    //Guardamos el bit en el bitmap de bloques
                    fseek(stream,SB.s_bm_block_start + bitBloque,SEEK_SET);
                    fwrite(&buffer,sizeof(char),1,stream);
                    //Sobreescribimos el SB bloque
                    SB.s_free_inodes_count = SB.s_free_inodes_count - 1;
                    SB.s_free_blocks_count = SB.s_free_blocks_count - 2;
                    SB.s_first_ino = SB.s_first_ino + 1;
                    SB.s_first_blo = SB.s_first_blo + 2;
                    fseek(stream,SesionActual.PosicionInicialSB,SEEK_SET);
                    fwrite(&SB,sizeof(SuperBloque),1,stream);
                    return 1;
                }else
                    return 2;
            }

        }
    }else{//Es un directorio '/home/usac/archivos'
        //Verificar que exista el directorio
        int existe = buscarCarpetaArchivo(stream,directorio);
        if(existe == -1){
            if(P){
                int index = 0;
                string aux = "";
                //Crear posibles carpetas inexistentes
                for(int i = 0; i < cont; i++){
                    aux += "/"+lista.at(i);
                    char dir[500];
                    char auxDir[500];
                    strcpy(dir,aux.c_str());
                    strcpy(auxDir,aux.c_str());
                    int carpeta = buscarCarpetaArchivo(stream,dir);
                    if(carpeta == -1){
                        response = nuevaCarpeta(stream,fit,false,auxDir,index);
                        if(response == 2)
                            break;
                        strcpy(auxDir,aux.c_str());
                        index = buscarCarpetaArchivo(stream,auxDir);
                    }else
                        index = carpeta;
                }
            }else
                return 3;
        }else{//Solo crear la carpeta en el directorio
            char dir[100] = "/";
            strcat(dir,nombreCarpeta);
            return nuevaCarpeta(stream,fit,false,dir,existe);
        }
    }

    return response;
}

/*
Funcion que retorna una carpeta que se llena automaticamente con -1
*/
Bloque_Carpetas crearBloqueCarpeta(){
    Bloque_Carpetas carpeta;

    for(int i = 0; i < 4; i++){
        strcpy(carpeta.b_content[i].b_name,"");
        carpeta.b_content[i].b_inodo = -1;
    }

    return carpeta;
}

/*
funcion que devuelve los directorios delimitados por el separador /
*/
string getDirectorio(string aux){
    string delimiter = "/";
    size_t pos = 0;
    string res = "";
    while((pos = aux.find(delimiter))!=string::npos){
        res += aux.substr(0,pos)+"/";
        aux.erase(0,pos + delimiter.length());
    }
    return res;
}

/*
Este metodo es el principal para crear carpetas, recibe el nombre del directorio como tal, la ruta donde se creara y la bandera P Por si se desea crear las carpetas que no existen
*/
void MKDIR(string nameSinruta,string ruta,bool valP){
    string name = nameSinruta;
    if(name.length() <= 11){
        if(SesionActiva){
            int CarpetaCreadaConExito = crearCarpeta(ruta,valP);
            if(CarpetaCreadaConExito == 0)
                cout << "\033[91m La carpeta ya existe!! \033[0m" << endl;
            else if(CarpetaCreadaConExito == 1){
                if(SesionActual.FS == 3){
                    char aux[500];
                    char comando[8];
                    char Contenido[5];
                    strcpy(aux,ruta.c_str());
                    strcpy(comando,"mkdir");
                    strcpy(Contenido,"null");
                    SaveJournaling(comando,1,664,aux,Contenido);
                }
                cout << "\033[96m Carpeta creada con exito :D \033[0m" << endl;
            }else if(CarpetaCreadaConExito == 2)
                cout << "\033[91m Error: no se tienen permisos de escritura \033[0m" << endl;
            else if(CarpetaCreadaConExito == 3){
                cout << "\033[91m El directorio del path no existe si quiere crearlo agrege el comando -P \033[0m" << endl;
            }
        }else
            cout << "\033[91m Error: Se necesita iniciar sesion para ejecutar el comando \033[0m" << endl;
    }
}

/*
Metodo que abre el archivo de la ruta donde se encuentra el disco y se poisicona en el SUPERBLOQUE, si existe retorna 0 sino se redirije hacia la nueva carpeta a crearla.
*/
int crearCarpeta(string path, bool p){
    FILE *FileDisk = fopen(SesionActual.direccion.c_str(),"rb+");

    SuperBloque SB;
    fseek(FileDisk,SesionActual.PosicionInicialSB,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);

    string aux = path;
    char Pathtemp[500];
    strcpy(Pathtemp,aux.c_str());
    int existe = buscarCarpetaArchivo(FileDisk,Pathtemp);
    strcpy(Pathtemp,aux.c_str());
    int retorno = -1;

    if(existe != -1)
        retorno = 0;
    else
        retorno = nuevaCarpeta(FileDisk,SesionActual.fit,p,Pathtemp,0);

    fclose(FileDisk);

    return retorno;
}

////////////////////////////////////////////////////////////////////////////////MANEJO DE REPORTES ///////////////////////////////////////////////////////////////////////////////////////////////////////
void REPORTES(string NombreReporte,string DestinoReporte,string IdentificadorParticionMontada,string ruta){
    //Recibo el ID en minusculas
    cout<<"nombre reporte:"<<NombreReporte<<endl;
    cout<<"carpeta destino reporte:"<<DestinoReporte<<endl;
    cout<<"id disco reporte:"<<IdentificadorParticionMontada<<endl;
    cout<<"ruta reporte file/ls:"<<ruta<<endl;
    string idtmp;
    bool flag = false;
    string extension = ObtenerExt(DestinoReporte);
    for(int i=0;i < arreglonodos.size();i++){
        string vd="vd";
        string letra="";
        letra = arreglonodos[i].letra;
        string numero = to_string(arreglonodos[i].numero);
        idtmp = vd+letra+numero;
        if(idtmp==IdentificadorParticionMontada){
            flag=true; 
            string directorio = getDirectorio(DestinoReporte);
            string comando = "sudo mkdir -p \'"+directorio+"\'";
            system(comando.c_str());
            string comando2 = "sudo chmod -R 777 \'"+directorio+"\'";
            system(comando2.c_str());
            if(NombreReporte == "MBR")
                GraficarMBR(arreglonodos[i].path,DestinoReporte,extension);
            else if(NombreReporte == "DISK")
                GraficarDISCO(arreglonodos[i].path,DestinoReporte,extension);
            else if(NombreReporte == "INODE"){
                 GraficarINODEYBLOCK(arreglonodos[i].path,DestinoReporte,extension,arreglonodos[i].name,NombreReporte);
            }else if(NombreReporte == "JOURNALING"){
                GraficarJOURNALING(arreglonodos[i].path,DestinoReporte,extension,arreglonodos[i].name);
            }else if(NombreReporte == "BLOCK"){
                GraficarINODEYBLOCK(arreglonodos[i].path,DestinoReporte,extension,arreglonodos[i].name,NombreReporte);
            }else if(NombreReporte == "BM_INODE"){
                cout<<"Graficare bm_inode :D"<<endl;
            }else if(NombreReporte == "BM_BLOCH"){
                cout<<"Graficare bm_block :D"<<endl;
            }else if(NombreReporte == "TREE"){
                cout<<"Graficare tree :D"<<endl;
            }else if(NombreReporte == "SB"){
                GraficarINODEYBLOCK(arreglonodos[i].path,DestinoReporte,extension,arreglonodos[i].name,NombreReporte);
            }else if(NombreReporte == "FILE"){
                cout<<"Graficare file :D"<<endl;
            }else if(NombreReporte == "LS"){
                cout<<"Graficare ls :D"<<endl;
            }      
        }
    }if(!flag){
        cout<<"\033[91m Error: No se encontro la partición montada con ese id!!!\n Las particiones montadas son: \033[0m"<<endl;
    }        
}

string ObtenerExt(string direccion){
    string aux = direccion;
    string delimiter = ".";
    size_t pos = 0;
    while((pos = aux.find(delimiter))!=string::npos){
        aux.erase(0,pos+delimiter.length());
    }
    return aux;
}

void GraficarMBR(string direccion, string destino, string extension){
    FILE *FileDis;
    FILE *FileGraphDot;
    //Empezamos a llenar el archivo para la grafica del mbr
    if((FileDis = fopen(direccion.c_str(),"r"))){
        FileGraphDot = fopen("MBR.dot", "w");
        fprintf(FileGraphDot,"digraph G{ \n");
        fprintf(FileGraphDot,"subgraph cluster_0{\n label=\"REPORTE MBR\"");
        fprintf(FileGraphDot,"\nrepmbr[shape=box3d,label=<\n");
        fprintf(FileGraphDot,"<table border=\'25\' cellborder=\'0\' cellspacing=\'15\' width=\'300\'  height=\'200\' color=\'#008B8B\' >\n");//Establecemos con html el borde 
        fprintf(FileGraphDot, "<tr>  <td width=\'150\'> <b>Atributo MBR</b> </td> <td width=\'150\'> <b>Valor</b> </td>  </tr>\n");
        MBR masterBoot;
        fseek(FileDis,0,SEEK_SET);
        fread(&masterBoot,sizeof(MBR),1,FileDis);
        int tamano = masterBoot.mbr_tamano;
        fprintf(FileGraphDot,"<tr>  <td><b>mbr_tamaño</b></td><td>%d</td>  </tr>\n",tamano);
        struct tm *tm;
        char date[100];
        tm = localtime(&masterBoot.mbr_fecha_creacion);
        /*
        Establecemos los subtitulos de los diferentes atributos para llenar
        */
        strftime(date,100,"%d/%m/%y %H:%S",tm);//Hora de generacion de mbr
        fprintf(FileGraphDot,"<tr>  <td><b>mbr_fecha_creacion</b></td> <td>%s</td>  </tr>\n",date);
        fprintf(FileGraphDot,"<tr>  <td><b>mbr_disk_signature</b></td> <td>%d</td>  </tr>\n",masterBoot.mbr_disk_signature);
        fprintf(FileGraphDot,"<tr>  <td><b>disk_fit</b></td> <td>%c</td>  </tr>\n",masterBoot.disk_fit);

        /*
        
        */
        int index_Extendida = -1;
        for (int i = 0; i < 4; i++){
            if(masterBoot.mbr_partition[i].part_start!=-1 && masterBoot.mbr_partition[i].part_status!='1'){
                if(masterBoot.mbr_partition[i].part_type == 'E'){
                    index_Extendida = i;//Por si es extendida.
                }
                /*
                Si no ex extendida creamos las filas llenando los datos con las partes de la primaria
                */
                char status[3];
                if(masterBoot.mbr_partition[i].part_status == '0')// copiamos el status de la particion
                    strcpy(status,"0");
                else if(masterBoot.mbr_partition[i].part_status == '2')// copiamos el status de la particion
                    strcpy(status,"2");
                fprintf(FileGraphDot,"<tr>  <td><b>part_status_%d</b></td> <td>%s</td>  </tr>\n",(i+1),status);
                fprintf(FileGraphDot,"<tr>  <td><b>part_type_%d</b></td> <td>%c</td>  </tr>\n",(i+1),masterBoot.mbr_partition[i].part_type);
                fprintf(FileGraphDot,"<tr>  <td><b>part_fit_%d</b></td> <td>%c</td>  </tr>\n",(i+1),masterBoot.mbr_partition[i].part_fit);
                fprintf(FileGraphDot,"<tr>  <td><b>part_start_%d</b></td> <td>%d</td>  </tr>\n",(i+1),masterBoot.mbr_partition[i].part_start);
                fprintf(FileGraphDot,"<tr>  <td><b>part_size_%d</b></td> <td>%d</td>  </tr>\n",(i+1),masterBoot.mbr_partition[i].part_size);
                fprintf(FileGraphDot,"<tr>  <td><b>part_name_%d</b></td> <td>%s</td>  </tr>\n",(i+1),masterBoot.mbr_partition[i].part_name);
            }
        }

        fprintf(FileGraphDot,"</table>\n");//Cerramos la tabla html
        fprintf(FileGraphDot, ">];\n}\n");//finalizamos el dot.

        /*
        Si la extendida existe osea es diferente de -1
        */
        if(index_Extendida != -1){
            int index_ebr = 1;
            EBR ebr;
            //posicionamos el arhivo donde inicia el mbr
            fseek(FileDis,masterBoot.mbr_partition[index_Extendida].part_start,SEEK_SET);
            /*
            Repetimos el ciclo hasta que se acaben las EBR activas
            */
            while(fread(&ebr,sizeof(EBR),1,FileDis)!=0 && (ftell(FileDis) < masterBoot.mbr_partition[index_Extendida].part_start + masterBoot.mbr_partition[index_Extendida].part_size)) {
                if(ebr.part_status != '1'){
                    /*
                    Si el estado del EBR es diferene de -1 repetimos 
                    */
                    fprintf(FileGraphDot,"subgraph cluster_%d{\n label=\"EBR_Numero_%d\"\n",index_ebr,index_ebr);
                    fprintf(FileGraphDot,"\nrepmbr_%d[shape=box3d,label=<\n ",index_ebr);//agregamos el numero del ebr a cada nodo
                    fprintf(FileGraphDot, "<table border=\'25\' cellborder=\'1\' cellspacing=\'15\'  width=\'300\' height=\'160\' color=\'#008B8B\' >\n ");
                    fprintf(FileGraphDot, "<tr>  <td width=\'150\'><b>Atributo EBR</b></td> <td width=\'150\'><b>Valor</b></td>  </tr>\n");
                    char status[3];
                    if(ebr.part_status == '0')//copiamos estado
                        strcpy(status,"0");
                    else if(ebr.part_status == '2')//copiamos estado
                        strcpy(status,"2");
                    fprintf(FileGraphDot, "<tr>  <td><b>part_status_1</b></td> <td>%s</td>  </tr>\n",status);
                    fprintf(FileGraphDot, "<tr>  <td><b>part_fit_1</b></td> <td>%c</td>  </tr>\n",ebr.part_fit);
                    fprintf(FileGraphDot, "<tr>  <td><b>part_start_1</b></td> <td>%d</td>  </tr>\n",ebr.part_start);
                    fprintf(FileGraphDot, "<tr>  <td><b>part_size_1</b></td> <td>%d</td>  </tr>\n",ebr.part_size);
                    fprintf(FileGraphDot, "<tr>  <td><b>part_next_1</b></td> <td>%d</td>  </tr>\n",ebr.part_next);
                    fprintf(FileGraphDot, "<tr>  <td><b>part_name_1</b></td> <td>%s</td>  </tr>\n",ebr.part_name);
                    fprintf(FileGraphDot, "</table>\n");
                    fprintf(FileGraphDot, ">];\n}\n");
                    index_ebr++;//Sumamos para pasar al siguiente ebr
                }

                if(ebr.part_next == -1)//paramos en dado caso la logica este en estado -1
                    break;
                else
                    fseek(FileDis,ebr.part_next,SEEK_SET);//Posicionamos el puntero del ardchivo al siguiente
            }
        }
        fprintf(FileGraphDot,"}\n");//cerramos el dot
        fclose(FileGraphDot);//cerramos el file del dot
        fclose(FileDis);//cerramos archivo del disco
        string comando = "dot -T"+extension+" MBR.dot -o "+destino;
        system(comando.c_str());//ejecutamos el comando para crear el dot y ejecutarlo 
        cout << "\033[96m Reporte MBR generado con éxito :D \033[0m " << endl;
    }
}

/*
Metodo llamado para graficar disco
*/
void GraficarDISCO(string PathDisk,string PathDestino, string extension ){
    FILE *FileDisk;
    FILE *FileGraphDot;
    if((FileDisk = fopen(PathDisk.c_str(),"r"))){//Verificamos si existe el disco

        /*
        Abrimos el archivo en Disk.dot en modo escritura, y empezamos el encabezado del dot a;adiendo el inicio de la tabla para el reporte
        */
        FileGraphDot = fopen("Disk.dot","w");
        fprintf(FileGraphDot,"digraph G{\n\n");
        fprintf(FileGraphDot, "repdisk[\nshape=box3d\n label=<\n");
        fprintf(FileGraphDot, "<table border=\'10\' cellborder=\'1\' width=\'600\' height=\"200\" color=\'#20B2AA\'>\n");
        fprintf(FileGraphDot, "<tr>\n");
        fprintf(FileGraphDot, "<td height=\'200\' width=\'100\'> MBR </td>\n");
        MBR mbr;
        /*
        Apuntamos el mbr al inicio y le asignamos el tamanio del mbr para luego leerlo completo
        */
        fseek(FileDisk,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,FileDisk);
        int Tamanio_mbr = mbr.mbr_tamano;// igualamos la variable al tamanio del mbr total
        double Usado = 0;//declaramos variable para ver cuanto se va usando nos servira para los porcentajes
        for(int i = 0; i < 4; i++){//Recorremos las 4 particiones posibles
            int a = mbr.mbr_partition[i].part_size;//Igualamos al tamanio de la particion
            if(mbr.mbr_partition[i].part_start != -1){//Particion no vacia
                double RealPorcentaje = (a*100)/Tamanio_mbr;//Sacamos el porcentaje utilizado del disco
                double ancho = (RealPorcentaje*500)/100;
                /*
                Estos 2 porcentajes nos serviran para imprimir el porcentaje utilizado y para dal el tamanio al td.
                */
                Usado += RealPorcentaje;
                if(mbr.mbr_partition[i].part_status != '1'){
                    if(mbr.mbr_partition[i].part_type == 'P'){
                        fprintf(FileGraphDot, "     <td height=\'200\' width=\'%.1f\'>PRIMARIA <br/> Ocupado: %.1f%c</td>\n",ancho,RealPorcentaje,'%');
                        //Verificar que no haya espacio fragmentado
                        if(i!=3){
                            int p1 = mbr.mbr_partition[i].part_start + mbr.mbr_partition[i].part_size;
                            int p2 = mbr.mbr_partition[i+1].part_start;
                            if(mbr.mbr_partition[i+1].part_start != -1){
                                if((p2-p1)!=0){//Hay fragmentacion
                                    int fragmentacion = p2-p1;
                                    double RealPorcentaje = (fragmentacion*100)/Tamanio_mbr;
                                    double ancho = (RealPorcentaje*500)/100;
                                    fprintf(FileGraphDot,"<td height=\'200\' width=\'%.1f\'>LIBRE<br/> Ocupado: %.1f%c</td>\n",ancho,RealPorcentaje,'%');
                                }
                            }

                        }else{
                            int p1 = mbr.mbr_partition[i].part_start + mbr.mbr_partition[i].part_size;
                            int mbr_tamano = Tamanio_mbr + (int)sizeof(MBR);
                            if((mbr_tamano-p1)!=0){
                                double libre = (mbr_tamano - p1) + sizeof(MBR);
                                double RealPorcentaje = (libre*100)/Tamanio_mbr;
                                double ancho = (RealPorcentaje*500)/100;
                                fprintf(FileGraphDot, "     <td height=\'200\' width=\'%.1f\'>LIBRE<br/> Ocupado: %.1f%c</td>\n",ancho, RealPorcentaje, '%');
                            }
                        }
                    }else{//si hay extendida 
                        EBR ebr;
                        /*
                        Escribimos el encabezado para las extendidas
                        */
                        fprintf(FileGraphDot,"<td  height=\'200\' width=\'%.1f\'>\n     <table border=\'0\'  height=\'200\' WIDTH=\'%.1f\' cellborder=\'1\'>\n",RealPorcentaje,RealPorcentaje);
                        fprintf(FileGraphDot," <tr>  <td height=\'60\' colspan=\'15\'>EXTENDIDA</td>  </tr>\n     <tr>\n");
                        /*
                        Apuntamos en la posicon donde empieza la extendida y leemos todo el EBR
                        */
                        fseek(FileDisk, mbr.mbr_partition[i].part_start,SEEK_SET);
                        fread(&ebr,sizeof(EBR),1,FileDisk);
                        if(ebr.part_size != 0){//Aqui entrara si encuentra una logica 
                            /*
                            Apuntamos el archivo a la particion extendida y vamos leyendo ebr por ebr hasta que se vaan acabando las logicas 
                            */
                            fseek(FileDisk, mbr.mbr_partition[i].part_start,SEEK_SET);
                            while(fread(&ebr,sizeof (EBR),1,FileDisk)!=0 && (ftell(FileDisk) < (mbr.mbr_partition[i].part_start + mbr.mbr_partition[i].part_size))){
                                a = ebr.part_size;
                                RealPorcentaje = (a*100)/Tamanio_mbr;
                                if(RealPorcentaje!=0){
                                    if(ebr.part_status != '1'){
                                        fprintf(FileGraphDot, "<td height=\'140\'>EBR</td>\n");
                                        fprintf(FileGraphDot, "<td height=\'140\'>LOGICA<br/>Ocupado: %.1f%c</td>\n",RealPorcentaje,'%');
                                    }else{//Espacio no asignado
                                        fprintf(FileGraphDot, "<td height=\'150\'>LIBRE 1 <br/> Ocupado: %.1f%c</td>\n",RealPorcentaje,'%');
                                    }
                                    if(ebr.part_next==-1){
                                        a = (mbr.mbr_partition[i].part_start + mbr.mbr_partition[i].part_size) - (ebr.part_start + ebr.part_size);
                                        RealPorcentaje = (a*100)/Tamanio_mbr;
                                        if(RealPorcentaje!=0){
                                            fprintf(FileGraphDot, " <td height=\'150\'>LIBRE 2<br/> Ocupado: %.1f%c </td>\n",RealPorcentaje,'%');
                                        }
                                        break;
                                    }else
                                    //Apuntamos hacia la siguiente logica para cuadno vuelva a entrar al while
                                        fseek(FileDisk,ebr.part_next,SEEK_SET);
                                }
                            }
                        }else{
                            fprintf(FileGraphDot,"<td height=\'140\'> Ocupado %.1f%c</td>",RealPorcentaje,'%');//Por si no hay logicas
                        }
                        fprintf(FileGraphDot,"</tr>\n</table>\n</td>\n");//Cerramos nuestras etiquetas
                        //Verificar que no haya espacio fragmentado
                        if(i!=3){
                            int p1 = mbr.mbr_partition[i].part_start + mbr.mbr_partition[i].part_size;
                            int p2 = mbr.mbr_partition[i+1].part_start;
                            if(mbr.mbr_partition[i+1].part_start != -1){
                                if((p2-p1)!=0){//Hay fragmentacion
                                    int fragmentacion = p2-p1;
                                    double RealPorcentaje = (fragmentacion*100)/Tamanio_mbr;
                                    double ancho = (RealPorcentaje*500)/100;
                                    fprintf(FileGraphDot,"<td height=\'200\' width=\'%.1f\'>LIBRE<br/> Ocupado: %.1f%c</td>\n",ancho,RealPorcentaje,'%');
                                }
                            }
                        }else{
                            int p1 = mbr.mbr_partition[i].part_start + mbr.mbr_partition[i].part_size;
                            int mbr_tamano = Tamanio_mbr + (int)sizeof(MBR);
                            if((mbr_tamano-p1)!=0){//Libre
                                double libre = (mbr_tamano - p1) + sizeof(MBR);
                                double RealPorcentaje = (libre*100)/Tamanio_mbr;
                                double ancho = (RealPorcentaje*500)/100;
                                fprintf(FileGraphDot, "<td height=\'200\' width=\'%.1f\'>LIBRE<br/> Ocupado: %.1f%c</td>\n",ancho, RealPorcentaje, '%');
                            }
                        }
                    }
                }else{//Espacio no asignado
                     fprintf(FileGraphDot,"<td height=\'200\' width=\'%.1f\'>LIBRE <br/> Ocupado: %.1f%c</td>\n",ancho,RealPorcentaje,'%');
                }
            }
        }

        fprintf(FileGraphDot,"</tr>\n</table> \n>];\n\n}");//Cerramos etiquetas principales del dot
        fclose(FileGraphDot);//Cerramos archivo dot luego de escribirlo
        fclose(FileDisk);//Cerramos el archivo del disco
        string comando = "dot -T"+extension+" Disk.dot -o "+PathDestino;
        system(comando.c_str());//Ejecutamos el .dot para crear la imagen.
        cout << "\033[96m Reporte DISCO generado con éxito :D \033[0m  " << endl;
    }else{
        cout << "\033[91m Error: No se encontro el disco deseado para hacer el reporte\033[0m" << endl;
}
}
/*
Metodo que recorre el disco para extaer los datos de los SB bloques tambien utilizado para los bloques para reutilizar codigo...
*/
void GraficarINODEYBLOCK(string PathDisk,string PathDestino,string extension,string nombre,string operacion){
    /*
    Buscamos el indice donde se encuentran las primarias o extendidas.
    */
    int Numero = FindPrimariaYExtendida(PathDisk,nombre);
    if(Numero != -1){
        MBR mbr;
        SuperBloque SB;
        /*
        Abrimos el archivo de el disco y leemos todo el mbr
        */
        FILE *FileDisk = fopen(PathDisk.c_str(),"rb+");
        fread(&mbr,sizeof(MBR),1,FileDisk);
        /*
        Apuntamos el archivo en la posicion de la particion primaria  o logica encontrada y leemos el superbloque.
        */
        fseek(FileDisk,mbr.mbr_partition[Numero].part_start,SEEK_SET);
        if(operacion == "SB"){
            generarDotSB(PathDisk,PathDestino,extension,mbr.mbr_partition[Numero].part_start);
        }
        fread(&SB,sizeof(SuperBloque),1,FileDisk);//Leemos el SB bloque para mandar sus atributos al metodo que genera el dot
        fclose(FileDisk);//Cerramos el archivo del disco
        if(operacion=="INODE"){
            generarDotINODE(PathDisk,PathDestino,extension,SB.s_bm_inode_start,SB.s_inode_start,SB.s_bm_block_start);//Mandamos el SB bloque para leer los inodos en el siguiente metodo, de particiones primarias y extendidas
        }else if(operacion=="BLOCK"){
            generarDotBLOCK(PathDisk,PathDestino,extension,SB.s_bm_block_start,SB.s_block_start,SB.s_inode_start);
        }
         }else{//Si es logica 
        int Numero = FindLogic(PathDisk,nombre);
        if(Numero != -1){
            /*
            Realizamos la misma accion de arriba solo que apuntamos hacia el EBR y leemos el superbloque de la logica para enviarlo al siguiente metodo
            */
            EBR ebr;
            SuperBloque SB;
            FILE *FileDisk = fopen(PathDisk.c_str(),"rb+");
            fseek(FileDisk,Numero,SEEK_SET);
            fread(&ebr,sizeof(EBR),1,FileDisk);
            if(operacion=="SB"){
                int inicio = static_cast<int>(ftell(FileDisk));
                generarDotSB(PathDisk,PathDestino,extension,inicio);
            }
            fread(&SB,sizeof(SuperBloque),1,FileDisk);
            fclose(FileDisk);

           if(operacion=="INODE"){
            generarDotINODE(PathDisk,PathDestino,extension,SB.s_bm_inode_start,SB.s_inode_start,SB.s_bm_block_start);//Mandamos el SB bloque para leer los inodos en el siguiente metodo, de particiones primarias y extendidas
        }else if(operacion=="BLOCK"){
            generarDotBLOCK(PathDisk,PathDestino,extension,SB.s_bm_block_start,SB.s_block_start,SB.s_inode_start);
        }
        }
    }
}
/*
Método que recibe datos de superbloque de las particiones para extraer los datos de los inodos almacenados.
*/
void generarDotINODE(string PathDisk, string PathDestino, string extension,int bm_inode_start,int inode_start,int bm_block_start){
    FILE *FileDisk = fopen(PathDisk.c_str(), "r");

    Tabla_Inodos inodo;//Creamos un objeto de tipo de tabla de inodos para leer
    int InicioBitmapInodos = bm_inode_start;
    int i = 0;
    char buffer;
    /*
    Creamos el archivo dot donde generaremos el codigo..
    */
    FILE *GraphDot = fopen("Inode.dot","w");
    fprintf(GraphDot,"digraph G{\n\n");
    /*
    Se repetira mientras El inicio del bitmap de inodos sea menor a el inicio del bitmap de bloques
    */
    while(InicioBitmapInodos < bm_block_start){
        /*
        Apuntamos el archivo del disco al inicio del bitmap de inodos y aumentamos al siguiente bitmap
        */
        fseek(FileDisk,bm_inode_start + i,SEEK_SET);
        buffer = static_cast<char>(fgetc(FileDisk));//para verificar que no este vacio.
        InicioBitmapInodos++;
        if(buffer == '1'){
            /*
            Posicionamos el puntero en el inicio del inodo para leerlo y asi poder obtener los datos 
            */
            fseek(FileDisk,inode_start + static_cast<int>(sizeof(Tabla_Inodos))*i,SEEK_SET);
            fread(&inodo,sizeof(Tabla_Inodos),1,FileDisk);
            fprintf(GraphDot, "    inodo_%d [ shape=folder label=<\n",i);
            fprintf(GraphDot, "   <table border=\'10\' cellborder=\'1\' cellspacing=\'2\' bgcolor=\"gray\">");
            fprintf(GraphDot, "    <tr> <td colspan=\'2\'> <b>Inodo %d</b> </td></tr>\n",i);
            fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> i_uid </td> <td bgcolor=\"#F5F5F5\"> %d </td>  </tr>\n",inodo.i_uid);
            fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> i_gid </td> <td bgcolor=\"#F5F5F5\"> %d </td>  </tr>\n",inodo.i_gid);
            fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> i_size </td> <td bgcolor=\"#F5F5F5\"> %d </td> </tr>\n",inodo.i_size);

            //Obtenemos los tiempos solicitados  y los imprimimos en el dot.
            struct tm *tm;//Estructura de tipo tiempo
            char date[100];
            tm=localtime(&inodo.i_atime);
            strftime(date,100,"%d/%m/%y %H:%S",tm);
            fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> i_atime </td> <td bgcolor=\"#F5F5F5\"> %s </td>  </tr>\n",date);
            tm=localtime(&inodo.i_ctime);
            strftime(date,100,"%d/%m/%y %H:%S",tm);
            fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> i_ctime </td> <td bgcolor=\"#F5F5F5\"> %s </td>  </tr>\n",date);
            tm=localtime(&inodo.i_mtime);
            strftime(date,100,"%d/%m/%y %H:%S",tm);
            fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> i_mtime </td> <td bgcolor=\"#F5F5F5\"> %s </td></tr>\n",date);
            /*
            recorremos para los apuntadores directos...
            */
            for(int b = 0; b < 15; b++)
                fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> i_block_%d </td> <td bgcolor=\"#F5F5F5\"> %d </td> </tr>\n",b,inodo.i_block[b]);
            fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> i_type </td> <td bgcolor=\"#F5F5F5\"> %c </td> </tr>\n",inodo.i_type);
            fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> i_perm </td> <td bgcolor=\"#F5F5F5\"> %d </td> </tr>\n",inodo.i_perm);
            fprintf(GraphDot, "   </table>>]\n");
        }
        i++;
    }
    fprintf(GraphDot,"\n}");
    fclose(GraphDot);

    fclose(FileDisk);

    string comando = "dot -T"+extension+" Inode.dot -o "+PathDestino;
    system(comando.c_str());
    cout << "\033[96m Reporte Inodos generado con éxito :D \033[0m   " << endl;
}

/*
Metodos que se utilizan para la creacion de un reporte para el jorunaling.
*/

void GraficarJOURNALING(string PathDisk,string PathDestino,string extension,string nombre){
    //Buscamos la posicion de la particion
    int Cont = FindPrimariaYExtendida(PathDisk,nombre);
    if(Cont != -1){//Primaria|Extendida
        MBR mbr;
        SuperBloque SB;
        /*
        Abrimos el disco para obtener el SB bloque y de ahi obtener la posicion donde empieza   y la enviamos para el otro metodo
        */
        FILE *FileDisk = fopen(PathDisk.c_str(),"rb+");
        fread(&mbr,sizeof(MBR),1,FileDisk);
        fseek(FileDisk,mbr.mbr_partition[Cont].part_start,SEEK_SET);
        fread(&SB,sizeof(SuperBloque),1,FileDisk);
        fclose(FileDisk);
        generarDotJOURNALING(PathDisk,PathDestino,extension,mbr.mbr_partition[Cont].part_start);
    }
}

void generarDotJOURNALING(string direccion,string destino, string extension,int BitSBInicio){
   
   /*
   Abrimos el disco y creamos un objeto de tipo superbloque y de tipo Journaling
   */
    FILE *FileDisk = fopen(direccion.c_str(),"r");

    SuperBloque SB;
    Journaling Journal;

    /*
    Como del metodo anterior ya obtuvimos el bit de inicio del sb colocamos la lectura en ese bit y leemos el superbloque entero
    */
    fseek(FileDisk,BitSBInicio,SEEK_SET);
    fread(&SB,sizeof(SuperBloque),1,FileDisk);
    /*
    Creamos el archivo que almacenara el dot y tambien lo ejecutara para exportarlo en imagen
    */
    FILE *GraphDot = fopen("Journaling.dot","w");
    /*
    Creamos el inicio del archivo y empezamos a hacer la tabla 
    */
    fprintf(GraphDot,"digraph G{\n");
    fprintf(GraphDot, "    nodo [shape=note, label=<\n");
    fprintf(GraphDot, "   <table border=\'10\' cellborder='1\' cellspacing=\'5\'>\n");
    fprintf(GraphDot, "    <tr> <td COLSPAN=\'7\' bgcolor=\"gray\"> <b>REPORTE JOURNALING</b> </td></tr>\n");
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"><b>Operacion</b></td> <td bgcolor=\"gray\"><b>Tipo</b></td><td bgcolor=\"gray\"><b>Nombre</b></td><td bgcolor=\"gray\"><b>Contenido</b></td>\n");
    fprintf(GraphDot, "    <td bgcolor=\"gray\"><b>Propietario</b></td><td bgcolor=\"gray\"><b>Permisos</b></td><td bgcolor=\"gray\"><b>Fecha</b></td></tr>\n");
    
    /*
    posicionamos en el bit del inicio
    */
    fseek(FileDisk,BitSBInicio + static_cast<int>(sizeof(SuperBloque)),SEEK_SET);
    while(ftell(FileDisk) < SB.s_bm_inode_start){
        /*
        recorremos para ir leyendo y sustituyendo datos
        */
        fread(&Journal,sizeof(Journaling),1,FileDisk);
        if(Journal.Journal_tipo == 0 || Journal.Journal_tipo == 1){
            struct tm *tm;
            char Date[100];
            tm = localtime(&Journal.Journal_fecha);
            strftime(Date,100,"%d/%m/%y %H:%S",tm);
            fprintf(GraphDot,"<tr><td>%s</td><td>%d</td><td>%s</td><td>%s</td><td>%d</td><td>%d</td><td>%s</td></tr>\n",Journal.Journal_tipo_operacion,Journal.Journal_tipo,Journal.Journal_nombre,Journal.Journal_contenido,Journal.Journal_propietario,Journal.Journal_permisos,Date);
        }
    }
    fprintf(GraphDot, " </table>>]\n");
    fprintf(GraphDot,"}");
    fclose(GraphDot);

    fclose(FileDisk);

    string comando = "dot -T"+extension+" Journaling.dot -o "+destino;
    system(comando.c_str());
    cout << "\033[96m Reporte Journal generado con éxito :D \033[0m " << endl;
}


/*
Metodo que crea el .dot para generar la grafica del bloque
*/
void generarDotBLOCK(string PathDisk, string PathDestino, string extension, int bm_block_start, int block_start, int inode_start){
    /*
    Iniiamos el disco (abrimos) y cremos 3 objetos de los cuales esta compuesto el bloque, que son carpeta, archivo y apuntadores.
    */
    FILE *FileDisk = fopen(PathDisk.c_str(),"r");
    Bloque_Carpetas carpeta;
    Bloque_Archivos archivo;
    BloqueDeApuntadores Puntero;

    int InicioBitMapBlock = bm_block_start;
    int i = 0;
    char buffer;

    /*
    Abrimos el archivo donde crearemos el dot 
    */
    FILE *GraphDot = fopen("Block.dot","w");
    fprintf(GraphDot,"digraph G{\n\n");
    /*
    Se repetira mientras el inicio del bitmap sea menor al bit inicial del inodo
    */
    while(InicioBitMapBlock < inode_start){
        /*
        Posicionamos el archivo del disco en el punto inicial del bloque
        */
        fseek(FileDisk,bm_block_start + i,SEEK_SET);
        buffer = static_cast<char>(fgetc(FileDisk));
        InicioBitMapBlock++;
        if(buffer == '1'){//mientras sea de tipo bloque carpeta
            /*
            posicionamos al inicio del bloque decarpetas
            leemos el bloque de carpetas
            */
            fseek(FileDisk,block_start + static_cast<int>(sizeof(Bloque_Carpetas))*i,SEEK_SET);
            fread(&carpeta,sizeof(Bloque_Carpetas),1,FileDisk);
            /*
            empezamos a escribir en el dot ya que podemos obtener los datos necesarios.
            */
            fprintf(GraphDot, "    nodo_%d [ shape=tab,  label=< \n",i);
            fprintf(GraphDot, "   <table border=\'0\' cellborder='1' cellspacing='0' bgcolor=\"gray\">");
            fprintf(GraphDot, "    <tr> <td colspan=\'2\'> <b>Rep Bloque Carpetas_ %d</b> </td></tr>\n",i);
            fprintf(GraphDot, "    <tr> <td bgcolor=\"#708090\"> b_name </td> <td bgcolor=\"#708090\"> b_inode </td></tr>\n");
            
            for(int j = 0;j < 4;j++)
                fprintf(GraphDot, "    <tr> <td bgcolor=\"#FFF5EE\"> %s </td> <td bgcolor=\"#FFF5EE\"> %d </td></tr>\n",carpeta.b_content[j].b_name,carpeta.b_content[j].b_inodo);
            fprintf(GraphDot, "   </table>>]\n\n");
            /*
            En el caso de ue el buffer sea 2 o 3 siguen la misma logica ue cuando es tipo bloque carpeta!!
            */
        }else if(buffer == '2'){//mientras sea de tipo bloque archivo
            fseek(FileDisk,block_start + static_cast<int>(sizeof(Bloque_Archivos))*i,SEEK_SET);
            fread(&archivo,sizeof(Bloque_Archivos),1,FileDisk);
            fprintf(GraphDot, "    nodo_%d [ shape=tab, label=< \n",i);
            fprintf(GraphDot, "   <table border=\'0\' cellborder='1' cellspacing='0' bgcolor=\"#8B0000\">");
            fprintf(GraphDot, "    <tr> <td colspan=\'2\'> <b>Bloque Archivo %d </b></td></tr>\n",i);
            fprintf(GraphDot, "    <tr> <td colspan=\'2\' bgcolor=\"#FFF5EE\"> %s </td></tr>\n",archivo.b_content);
            fprintf(GraphDot, "   </table>>]\n\n");
        }else if(buffer == '3'){//Mientras sea de tipo apuntadores 
            fseek(FileDisk,block_start + static_cast<int>(sizeof(BloqueDeApuntadores))*i,SEEK_SET);
            fread(&Puntero,sizeof(BloqueDeApuntadores),1,FileDisk);
            fseek(FileDisk,block_start + static_cast<int>(sizeof(BloqueDeApuntadores))*i,SEEK_SET);
            fread(&Puntero,sizeof(BloqueDeApuntadores),1,FileDisk);
            fprintf(GraphDot, "    bloque_%d [shape=plaintext  label=< \n",i);
            fprintf(GraphDot, "   <table border=\'0\' bgcolor=\"#FF0000\">\n");
            fprintf(GraphDot, "    <tr> <td> <b>Bloque de punteros %d</b></td></tr>\n",i);
            for(int j = 0; j < 16; j++)
                fprintf(GraphDot, "    <tr> <td bgcolor=\"#FFF5EE\">%d</td> </tr>\n",Puntero.b_pointers[j]);
            fprintf(GraphDot, "   </table>>]\n\n");
        }
        i++;//contador de indexacion
    }

    fprintf(GraphDot,"\n}");
    fclose(GraphDot);

    fclose(FileDisk);

    string comando = "dot -T"+extension+" Block.dot -o "+PathDestino;
    system(comando.c_str());
    cout << "\033[96m Reporte Block generado con éxito :D \033[0m " << endl;
}



void generarDotSB(string direccion, string destino, string extension, int start_super){
    /*
    Abrimos el disco y creamos un super bloque
    */
    FILE* FileDisk = fopen(direccion.c_str(),"r");

    SuperBloque SB;
    /*
    Establecemos el puntero en el inicio del superbloque y leemos el superbloque completo
    */
    fseek(FileDisk,start_super,SEEK_SET);
    fread(&SB,sizeof (SB),1,FileDisk);

    /*
    Creamos el archivo que almacenara eldot que generara el reporte ya que ya tenemos acceso para leer los datos...
    */
    FILE *GraphDot = fopen("SuperBloque.dot","w");
    fprintf(GraphDot,"digraph G{\n");
    fprintf(GraphDot, "    nodo [shape=box3d, label=<");
    fprintf(GraphDot, "   <table border=\'10\' cellborder='0\' cellspacing=\'2\' bgcolor=\"cornflowerblue\">");
    fprintf(GraphDot, "    <tr> <td COLSPAN=\'2\'> <b>SUPERBLOQUE</b> </td></tr>\n");
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_inodes_count </td> <td bgcolor=\"#F0FFF0\"> %d </td> </tr>\n",SB.s_inodes_count);
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_blocks_count </td> <td bgcolor=\"#F0FFF0\"> %d </td> </tr>\n",SB.s_blocks_count);
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_free_block_count </td> <td bgcolor=\"#F0FFF0\"> %d </td> </tr>\n",SB.s_free_blocks_count);
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_free_inodes_count </td> <td bgcolor=\"#F0FFF0\"> %d </td> </tr>\n",SB.s_free_inodes_count);
    /*
    Creamos una estructura de tipo time para poder indicar los parametros que son de tiempo
    */
    struct tm *tm;
    char fecha[100];
    tm=localtime(&SB.s_mtime);
    strftime(fecha,100,"%d/%m/%y %H:%S",tm);
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_mtime </td> <td bgcolor=\"#F0FFF0\"> %s </td></tr>\n",fecha);
    tm=localtime(&SB.s_umtime);
    strftime(fecha,100,"%d/%m/%y %H:%S",tm);
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_umtime </td> <td bgcolor=\"#F0FFF0\"> %s </td> </tr>\n",fecha);
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_mnt_count </td> <td bgcolor=\"#F0FFF0\"> %d </td> </tr>\n",SB.s_mnt_count);
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_magic </td> <td bgcolor=\"#F0FFF0\"> %d </td> </tr>\n",SB.s_magic);
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_inode_size </td> <td bgcolor=\"#F0FFF0\"> %d </td> </tr>\n",SB.s_inode_size);
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_block_size </td> <td bgcolor=\"#F0FFF0\"> %d </td> </tr>\n",SB.s_block_size);
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_first_ino </td> <td bgcolor=\"#F0FFF0\"> %d </td> </tr>\n",SB.s_first_ino);
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_first_blo </td> <td bgcolor=\"#F0FFF0\"> %d </td> </tr>\n",SB.s_first_blo);
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_bm_inode_start </td> <td bgcolor=\"#F0FFF0\"> %d </td></tr>\n",SB.s_bm_inode_start);
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_bm_block_start </td> <td bgcolor=\"#F0FFF0\"> %d </td> </tr>\n",SB.s_bm_block_start);
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_inode_start </td> <td bgcolor=\"#F0FFF0\"> %d </td> </tr>\n",SB.s_inode_start);
    fprintf(GraphDot, "    <tr> <td bgcolor=\"gray\"> s_block_start </td> <td bgcolor=\"#F0FFF0\"> %d </td> </tr>\n",SB.s_block_start);
    fprintf(GraphDot, "   </table>>]\n");
    fprintf(GraphDot,"\n}");
    fclose(GraphDot);

    fclose(FileDisk);

    string comando = "dot -T"+extension+" SuperBloque.dot -o "+destino;
    system(comando.c_str());
    cout << "\033[96m Reporte SuperBloque generado con éxito :D \033[0m " << endl;
}
/*
Metodo que ejecuta el proyecto con un poco de logica por si viene EXEC 
*/
int main(int argc, char const *argv[])
{
    cout<<"\033[92m********************************************"<<endl;
    cout<<"**          HARD DISK SIMULATION          **"<<endl;
    cout<<"**               201905554                **"<<endl;
    cout<<"******************************************** \033[0m\n"<<endl;
    char Linea_Comando[200];
    
    while((string)Linea_Comando!="EXIT"){
        cout<<"\033[96mCommand :: \033[0m";
        cin.getline(Linea_Comando,200,'\n');
        char lineaSincasteo[200];
        strcpy(lineaSincasteo,Linea_Comando);      
        string tmp=CastearMayuscula(Linea_Comando);//Casteamos a mayuscula toda la linea para evitarnos problemas del case-insensitive.
        vector<string> ls= SplitSpace(tmp);
        vector<string> lsx=SplitSpace(lineaSincasteo);
        if(ls[0]=="EXEC"){
            vector<string> tmp,auxiliarSinCastear;
            tmp = Split(ls[1],"~:~");
            auxiliarSinCastear = Split(lsx[1],"~:~");
            if(tmp[0]=="-PATH"){
                string auxiliare = auxiliarSinCastear[1];
                string pathx;
                pathx = EliminarComillasYreemplazarEspacios(auxiliare);
                string ruta;
                ruta = path+pathx;
                leerscript(ruta);
            }
        }
        else{
            EjecutarComando(lineaSincasteo);
        }

    }    
    return EXIT_SUCCESS;
}