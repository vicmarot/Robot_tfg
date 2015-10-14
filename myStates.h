//-----------------------------------------------------
//ESTE ES EL ÚNICO FICHERO QUE HAY QUE MODIFICAR
//-----------------------------------------------------

#ifndef myStates_H
#define myStates_H


//Declaracion de las funciones
extern void lectura(void); // Espera 
extern void MOV(void);// movimiento
extern void MDerecho(void); // Derecha
extern void MIzquierdo(void); // IZQUIERDA
extern void Parada_D(void);
extern void Parada_I (void);




//Declaracion del nombre de ESTADOS y de EVENTOS
#define STATE1  		0x01
#define STATE2  		0x02
#define STATE3 	        0x03
#define STATE4          0x04
#define STATE5          0x05
#define	STATE6			0x06

#define EV_Mov          0x41
#define EV_D     		0x42
#define EV_time         0x43
#define EV_count2       0x44
#define EV_I   	        0x45	
#define EV_RW           0x46
#define EV_NULL         0x47
#define EV_ERROR    	0x48	
#define EV_PARADA_D     0x49
#define EV_PARADA_I		0x50

// Estructuras descriptivas de mi diagrama de flujo
const FSMClass::FSM_State_t FSM_State[] PROGMEM= {
// STATE,STATE_FUNC
{STATE1,lectura},
{STATE2,MOV},// estado incial de movimiento quizas no sea necesario y se pueda suprimir
{STATE3,MDerecho},// motor derecho
{STATE4,MIzquierdo}, // motor izquierdo
{STATE5,Parada_D},// parada motor DER
{STATE6,Parada_I},
};

const FSMClass::FSM_NextState_t FSM_NextState[] PROGMEM= {
// STATE,EVENT,NEXT_STATE


//   ESTADO 1: LECTURA    //
{STATE1,EV_Mov,STATE2},
{STATE1,EV_PARADA,STATE5},
//   ESTADO 2: MOVIMIENTO   //
{STATE2,EV_D,STATE3},
{STATE2,EV_I,STATE4},
{STATE2,EV_NULL,STATE1},
{STATE2,EV_ERROR,STATE1},
//     ESTADO 3: MDERECHO   //
{STATE3,EV_I,STATE4},//test
{STATE3,EV_D,STATE3},//sobre si mismo
{STATE3,EV_NULL,STATE1},
{STATE3,EV_ERROR,STATE1},
{STATE3,EV_PARADA,STATE5},
//    ESTADO 4: MIZQUIERDO    //
{STATE4,EV_D,STATE3},
{STATE4,EV_I,STATE4},//sobre si mismo
{STATE4,EV_NULL,STATE1},
{STATE4,EV_ERROR,STATE1},
{STATE4,EV_PARADA,STATE6},
//    ESTADO 5    //
{STATE5,EV_NULL,STATE1},
};

//{STATE3,EV_RW,STATE2},// forzamos regreso al estado 2
//{STATE4,EV_RW,STATE2},// forzamos regreso al estado 2


//Macros para el cálculo de los tamaños de las estructuras
//NO MODIFICAR
#define nStateFcn		sizeof(FSM_State)/sizeof(FSMClass::FSM_State_t)
#define nStateMachine		sizeof(FSM_NextState)/sizeof(FSMClass::FSM_NextState_t)

#endif
