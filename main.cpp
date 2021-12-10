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
//Metodos globales
void ParticionPrimaria(int,char,string,char,string);
void PaticionExtendida(int,char,string,char,string);
bool ExisteParticion(string,string);
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
        mbr.mbr_partition[i].part_start = -1;
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
        cout<<"Error: No se pudo eliminar el archivo\n"<<endl;
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
            cout<<"******       Script pausado!       ******\n"<<"******Presione Enter para continuar******\n";
            //pausamos
            pause = cin.get();
        }
        vector<string>lineSplit = SplitSpace(comandoCasteado);//Spliteamos por espacios
            if(lineSplit[0]=="MKDISK"){
                int sizeerror,fiterror,uniterror =0;
                bool unit,fit = false;//Variables booleanas que se estableceran por defecto si no se declaran en el comando si son false se pondran en automatico por defecto segun enunciado
                vector<string> auxiliar;
                for(size_t i=1; i < lineSplit.size(); i++){//Repetiremos tantas veces desde 1 hasta que termine cada uno de los comandos(se empieza de 1 ya que no tomamos en cuenta el comando MKDISK)
                    auxiliar = Split(lineSplit[i],"~:~");
                    if(auxiliar[0] == "-PATH"){//Si el comando es el -path entonces entrara a esta condicional
                        mkcarpetas(auxiliar[1]);//Creamos las carpetas con el path ingresado.
                        Disk1.path = auxiliar[1];//Asignamos el path al disco
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
                            cout<<"Error: Esta tratando de ingresar un ajuste no permitido"<<endl;
                        }    
                    }
                    else if(auxiliar[0]=="-UNIT"){//Asignamos la unidad al disco
                        if(auxiliar[1]=="M" || auxiliar[1]=="K"){
                            Disk1.unit = auxiliar[1];
                            unit = true;
                        }else{
                            uniterror = 1;
                            cout<<"Error: Esta tratando de ingresar una unidad no permitida para crear disco"<<endl;
                        }
                        
                    }
                    else if(auxiliar[0] == "-SIZE"){//Asignamos el tamanio al disco
                        if(stof(auxiliar[1])<=0){
                            sizeerror = 1;
                            cout<<"Error: El tamaño del disco no puede ser negativo o igual a cero"<<endl;
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
                    cout<<sizeerror<<"a"<<uniterror<<"b"<<fiterror<<"c"<<endl;
                    cout<<"No se pudo crear el disco!!"<<endl;
                }else{
                    CrearDisco(Disk1);//Creamos disco
                }                
            }else if(lineSplit[0]=="RMDISK"){
                string dir;
                vector<string> aux;
                aux = Split(lineSplit[1],"~:~");
                if(aux[0] == "-PATH"){
                    dir = path+aux[1];
                    borrardisco(dir); 
                }           
            }else if(lineSplit[0]=="FDISK"){
                cout<<"entre a fdisk"<<endl;
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
                    auxiliar = Split(lineSplit[i],"~:~");
                    if(auxiliar[0] == "-SIZE"){//Si el comando es el -path entonces entrara a esta condicional
                        if(stof(auxiliar[1])<=0){
                            sizeerror = 1;
                            cout<<"Error: Esta tratando de ingresar una unidad no permitida para crear particion"<<endl;
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
                            cout<<"Error: Esta tratando de ingresar una unidad no permitida para crear una particion"<<endl;
                        }    
                    }else if(auxiliar[0]=="-PATH"){
                        rutapart = auxiliar[1];    
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
                            cout<<"Error: Esta tratando de ingresar un tipo incompatible con las que se pueden crear particiones utiliza P,E o L"<<endl;
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
                            cout<<"Error: Esta tratando de ingresar un ajuste no permitido para la particion"<<endl;
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
                    cout<<"-----Datos Partición a Crear-----"<<endl;
                    cout<<"size: "<<sizepart<<endl;
                    cout<<"unit: "<<unitpart<<endl;
                    cout<<"ruta: "<<rutapart<<endl;
                    cout<<"fit: "<<fitpart<<endl;
                    cout<<"name: "<<namepart<<endl;
                    ParticionPrimaria(sizepart,unitpart,rutapart,fitpart,namepart);

                }else if(deftype){
                    if(typepart=='E'){
                        PaticionExtendida(sizepart,unitpart,rutapart,fitpart,namepart);
                    }else if(typepart=='L'){
                        cout<<"Soy particion logica :D"<<endl;
                    }
                }  
            }//agregar defaults y tambien posibles errores.
    }
}
vector<string> lineas;//Vector que almacenara las lineas
void leerscript(string ruta){//Metodo que recibe como parameto la ruta del script con comandos a ejecutar
    FILE *script;
    if((script = fopen(ruta.c_str(),"r"))){//Si se encuentra el archivo y se puede abrir con exito
        char line[200]="";//declaramos un arreglo de char para poder almacenar linea individualmente 
        memset(line,0,sizeof(line));
        while(fgets(line,sizeof line,script)){//Mientras se lea una linea con salto de linea se ejecutara el while  
            if(line[0]!='\n'){//Obtenemos el dato de cada linea
                cout <<"SCRIPT: "<< line << endl;//Imprimimos el comando
                lineas.push_back(line);//Pusheamos la linea al arreglo
                EjecutarComando(line);//Ejecutamos el comando
            }
            memset(line,0,sizeof(line));
        }
        fclose(script);//Cerramos el archivo.
    }else{
        cout << "Error al abrir el SCRIPT" << endl;
    }    
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
            cout<<"Error: No esta ingresando una unidad permitida para la creacion de la particion primaria!!"<<endl;
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
            cout<<"Espacio Disponible: "<<mbr.mbr_tamano-bytes << " Bytes" <<endl;
            cout<<"Espacio Necesario para particion primaria: "<<sizetmp<<" Bytes"<<endl;
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
                        fwrite(&mbr,sizeof(MBR),1,fileprimary);//Guardamos en el nuevo masterboot
                        fseek(fileprimary,mbr.mbr_partition[contadorParticion].part_start,SEEK_SET);//Guardamos los bytes de la particion
                        for(int i=0; i < sizetmp; i++){
                            fwrite(&buffer,1,1,fileprimary);
                        }
                        cout<<"\n Particion PRIMARIA creada con exito de tipo FIRST FIT!!!"<<endl;
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
                        fwrite(&mbr,sizeof(MBR),1,fileprimary);//Guardamos en el nuevo masterboot
                        fseek(fileprimary,mbr.mbr_partition[best].part_start,SEEK_SET);//Guardamos los bytes de la particion
                        for(int i=0; i < sizetmp; i++){
                            fwrite(&buffer,1,1,fileprimary);
                        }
                        cout<<"\n Particion PRIMARIA creada con exito de tipo BEST FIT!!!"<<endl;
                    }else if(mbr.disk_fit == 'W'){//WORST FIT
                        cout<<"Particion worst fit"<<endl;
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
                        fwrite(&mbr,sizeof(MBR),1,fileprimary);//Guardamos en el nuevo masterboot
                        fseek(fileprimary,mbr.mbr_partition[worst].part_start,SEEK_SET);//Guardamos los bytes de la particion
                        for(int i = 0; i < sizetmp; i++){
                            fwrite(&buffer,1,1,fileprimary);
                        }
                        cout <<"\n Particion PRIMARIA creada con exito de tipo WORST FIT!!!"<<endl;
                    }
                
                }else{
                    cout<<"Error: La particion ya existe!!"<<endl;
                }  
            }else{
                cout<<"Error: No hay tanto espacio disponible, intenta con una particion que no exceda el espacio libre"<<endl;
            }
        }else{
            cout<<"Error: Ya se posee el numero maximo de particiones que son 4, elimina una para poder crear otra particion primaria!!"<<endl;
        }
        fclose(fileprimary);//Cerramos disco.
    }else{
        cout<<"Error: No se pudo abrir el disco, es posible que no exista!!"<<endl;
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
                cout << "Espacio disponible para partición extendida: " << (mbrE.mbr_tamano - espacioUsado) <<" Bytes"<< endl;
                cout << "Espacio necesario para partición extendida:  " << BytesExtend << " Bytes" << endl;
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
                            cout << "\nParticion EXTENDIDA creada con exito de tipo FIRST FIT"<< endl;
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
                            //start
                            if(MejorPosicion == 0){
                                mbrE.mbr_partition[MejorPosicion].part_start = sizeof(mbrE);
                            }else{
                                mbrE.mbr_partition[MejorPosicion].part_start =  mbrE.mbr_partition[MejorPosicion-1].part_start + mbrE.mbr_partition[MejorPosicion-1].part_size;
                            }
                            mbrE.mbr_partition[MejorPosicion].part_size = BytesExtend;
                            mbrE.mbr_partition[MejorPosicion].part_status = '0';
                            strcpy(mbrE.mbr_partition[MejorPosicion].part_name,name.c_str());
                            //Se guarda de nuevo el MBR
                            fseek(fileDisk,0,SEEK_SET);
                            fwrite(&mbrE,sizeof(MBR),1,fileDisk);
                            //Se guarda la particion extendida
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
                            cout<< "\nParticion EXTENDIDA creada con exito de tipo BEST FIT"<< endl;
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
                            //start
                            if(PeorPosicion == 0){
                                mbrE.mbr_partition[PeorPosicion].part_start = sizeof(mbrE);
                            }else{
                                mbrE.mbr_partition[PeorPosicion].part_start =  mbrE.mbr_partition[PeorPosicion-1].part_start + mbrE.mbr_partition[PeorPosicion-1].part_size;
                            }
                            mbrE.mbr_partition[PeorPosicion].part_size = BytesExtend;
                            mbrE.mbr_partition[PeorPosicion].part_status = '0';
                            strcpy(mbrE.mbr_partition[PeorPosicion].part_name,name.c_str());
                            //Se guarda de nuevo el MBR
                            fseek(fileDisk,0,SEEK_SET);
                            fwrite(&mbrE,sizeof(MBR),1,fileDisk);
                            //Se guarda la particion extendida
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
                            cout<< "\nParticion EXTENDIDA creada con exito de tipo WORST FIT"<< endl;
                        }
                    }else{
                        cout << "Error: La particion ya existe!!" << endl;
                    }
                }else{
                    cout << "Error: No hay tanto espacio disponible, intenta con una particion que no exceda el espacio libre" << endl;
                }
            }else{
                cout << "Error: Ya se posee el numero maximo de particiones que son 4, elimina una para poder crear particion lógica!!" << endl;
            }
        }else{
            cout << "Error: Ya existe una partición de tipo EXTENDIDA" << endl;
        }
    fclose(fileDisk);
    }else{
        cout << "Error: No se pudo abrir el disco, es posible que no exista!!" << endl;
    }
}




int main(int argc, char const *argv[])
{
    cout<<"********************************************"<<endl;
    cout<<"**          HARD DISK SIMULATION          **"<<endl;
    cout<<"**               201905554                **"<<endl;
    cout<<"******************************************** \n"<<endl;
    char Linea_Comando[200];
    
    while((string)Linea_Comando!="EXIT"){
        cout<<"Command :: ";
        cin.getline(Linea_Comando,200,'\n');
        string tmp=CastearMayuscula(Linea_Comando);//Casteamos a mayuscula toda la linea para evitarnos problemas del case-insensitive.
        vector<string> ls= SplitSpace(tmp);
        if(ls[0]=="EXEC"){
            vector<string> aux;
            aux = Split(ls[1],"~:~");
            if(aux[0]=="-PATH"){
                string ruta;
                ruta = path+aux[1];
                leerscript(ruta);
            }
        }
        else{
            EjecutarComando(Linea_Comando);
        }

    }    
    return EXIT_SUCCESS;
}