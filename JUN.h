//
// Copyright (C) 2019 Volker Diehl
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License (LGPL)
// as published by the Free Software Foundation, either version 3 or
// any later version.
// 
// This header is distributed in the hope that it will be useful,
// but without ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
// GUN Lesser General License for more details. 
//
// You should have received a copy of the GNU Lesser Genreral Publc Licence
// along with this program. If not, see <http://www.gnu.org/licences/> 
// 
//
// Feedback, error reports, tips or suggestion improvement to
// diehl-der-volker@web.d
//
// Jurists force us to care about laws, why don't we force them to learn programming ?
//
//
//*************************************************************************************************
// JUN.h
// =====
//
//             Author: Volker Diehl
//        used Header: stdint.h complex.h stdarg.h stdlib.h ernno.h
//       used Libarys: glibc 64 Bit
//     Provessor / OS: Pentium 64-Bit, Linux
//                     ( needs Little-Endian, 8086/Pentium extendet Real Numbers 80 bit )
//
// Beschreibung:
// -------------
// Header for Implementation of a universal Function-Prototype with universal usability
// named JUN-Fumctions, and a lot of macros to work with. Endhances possibilties for working 
// with variadic Arguments. JUN-Fuctions was thought as an universal Prototype for
// eventhandlers but of course can used any way they are helpful.
//
// !!! ATTENTION !!! The correkt Behaviour of this Macro JUN_RNDGET() depends on a property
//                   of the Macro va_arg() (Header stdarg.h) whitch is not guaranteed by the
//                   Standart C Library. So it might not work on all systems !
//
// History of Changes:
// -------------------
// 26.10.2019 VD checkt out of an other of my Software Projects  
//
//*************************************************************************************************
#ifndef  _JUN_h_def_
#define  _JUN_h_def_

#include <stdint.h>  // fixed size integer definitions
#include <complex.h>
#include <stdarg.h>  // fuer variadic Funktionen
#include <stdlib.h>  // fuer realloc
#include <errno.h>   // fuer JUN_RALL

//*************************************************************************************************
// JUN: Functionspointer Universal-Variadic-Function 
//
// 1.Arg: DataPtr:   For indirect Transfer of a Data Structure
// 2.Arg: JUN_DIM:   Size (in items) of an Array whitch ist the last Member of the 
//                   Data Structur in DataPtr
// 3.Arg: ArgCnt:    Number of optional Arguments
// n*Arg: variadic:  Function-specific Arguments
// m*Arg: variadic:  Optional Arguments
//
// The complete Documentation is to large to be included in this Headerfile,
// so see the belongig Dokumentation
//*************************************************************************************************
// Typ for direct Return Value of JUN-Function 
// Union that contains all basic C Types to fit in 128 Bit
//=================================================================================================
typedef union _JUN_RETTYP_
{                                     long double RL;    complex double CD; 
   uint64_t  UQ[ 2]; int64_t  SQ[ 2];      double RD[2]; complex float  CS[2]; void* P[2];  
   uint32_t  UD[ 4]; int32_t  SD[ 4];      float  RS[4]; 
   uint16_t  UW[ 8]; int16_t  SW[ 8]; 
   uint8_t   UB[16]; int8_t   SB[16];     
} JUN_RETTYP;
//=================================================================================================

typedef  JUN_RETTYP ( *JUN) (void *const DataPtr, uint32_t JUN_DIM, uint16_t ArgCnt ,...); 

//=================================================================================================
// Macros for Implementation of a JUN-Function 
//=================================================================================================
// Macros for Head- and Ending-Code of a  JUN-Function:
//
// - JUN_START(Name,Macroparameter) : Deklaration and Head of a JUN-Funktion named "Name"
// - JUN_EXIT()                     : Return from a JUN-Funkt.  (return must not be used) 
// - JUN_END()                      : Endcode  of a JUN-Funktion  
//
// general Prototype for Implementation JUN_Function "Name" :
//     JUN_START(Name,other Macroparameters)
//     {
//       ... /* Code of JUN-Funktion */
//
//       if(Bedingung) JUN_EXIT() /* optional: Return from JUN-Function */
//
//       ... /* more Code of JUN-Function */
//     }
//     JUN_END()
//
// Macroparameter of JUN_START() :
// 1. Name   : Name of the JUN-Function to be  implemented 
// 2. DtaMo  : Modus of Data Transfer by  DataPtr:
//             -  NON: no       Data Transfer resp. Return
//             -  DIR: direct   Data Transfer and   Return
//             -  IND: indirect Data Transfer and   Return
//             -  RET: indirect                     Return
// 2. DTyp   : C-Typ of Data-Structur to be transferred in DataPtr   
// 3. DMTy   : C-Typ of last Member of Dtyp: Array of DMTy
// 4. RTyp   : C-Typ of Data-Structur to be returned via JUN_RP
// 5. OptMo  : Mode for setting Numer of optional Arguments
//             - UNDEF: Number opt. Arg has to be set by JUN_ADDOPT
//             - UNLIM: Number opt. Arg will be detected by property of last opt. Argument 
//             - TRANS: Number opt. Arg = Functionsparameter ArgCnt * ACMu
// 6. ACMu   : Number opt. Parameter per Group of opt. Para. 
// 7. FixCnt : Number of  function-specific Arguments
//=================================================================================================
// Definitions for Macroparameter DtaMo: Data-Transfer-Mode resp Value of JUN_GETDM()
//-------------------------------------------------------------------------------------------------
#define JUN_DMNON 0x00 /* no       Data Transfer resp. Return */
#define JUN_DMDIR 0x01 /* direct   Data Transfer and   Return */
#define JUN_DMRET 0x02 /* indirect Data Transfer and   Return */
#define JUN_DMIND 0x03 /* indirect                     Return */
//-------------------------------------------------------------------------------------------------
// Definition Macroparameter OptMo resp. Functionparameter ArgCnt
//-------------------------------------------------------------------------------------------------
#define JUN_UNLIM 0xFFFF /* Parameter ArgCnt: unlimited Number of Arguments */
#define JUN_UNDEF 0xFFFE /* Parameter ArgCnt: undefined Number of Arguments */
//-------------------------------------------------------------------------------------------------
// Definition Return Values of  Macro JUN_GETAM() 
//-------------------------------------------------------------------------------------------------
#define JUN_AMUNDEF 0x00 // Number opt. Arg undefined (not yet defined) 
#define JUN_AMUNLIM 0x01 // Number opt. Arg unlimited
#define JUN_AMTRANS 0x02 // Number opt. Arg is defined
//-------------------------------------------------------------------------------------------------
// Submacro of JUN_START(...) dependend on DtaMo:
// Definition of Pointer JUN_DP, JUN_RP and _Arrp_
// 
// ( Some Macros always need Pointers JUN_DP and JUN_RP to be defined, even when they should
//   not be defined according to the functionality of JUN-Funktions. So they are here
//   defined as const NULL-Pointers to avoid using them ) 
//-------------------------------------------------------------------------------------------------
#define _1JUN_DMNON_(DTyp,DMTy,RTyp) /* JUN_DMNON: No Data Transfer : undefined     */  \
       void *const  JUN_DP = NULL; void *const _Arrp_ = NULL; void *const JUN_RP = NULL;          
#define _1JUN_DMRET_(DTyp,DmTy,RTyp) /* JUN_DMRET: Return Only      : undefined     */  \
       void *const  JUN_DP = NULL; void *const _Arrp_ = NULL;       RTyp* JUN_RP = NULL;   
#define _1JUN_DMDIR_(DTyp,DMTy,RTyp) /* JUN_DMDIR: direct                           */  \
              DTyp* JUN_DP = NULL;       DMTy* _Arrp_ = NULL; void *const JUN_RP = NULL;       
#define _1JUN_DMIND_(DTyp,DMTy,RTyp) /* JUN_DMIND: indirect                         */  \
              DTyp* JUN_DP = NULL;       DMTy* _Arrp_ = NULL;       RTyp* JUN_RP = NULL;            
//-------------------------------------------------------------------------------------------------
// Submacro of JUN_START(...) dependend on DtaMo:
// Initializing JUN_DP and _Arrp_ by _Data_Ptr_
//------------------------------------------------------------------------------------------------- 
#define _Data_IPtr_ *((void**)_Data_Ptr_) /* For indirect Access DataPtr       */ 
#define _2JUN_DMNON_(DTyp) {} /* JUN_DMNON: No Data Transfer: dummy            */
#define _2JUN_DMRET_(DTyp) {} /* JUN_DMRET: Return only     : dummy            */
/* JUN_DMDIR: direct: JUN_DP = DataPtr                                         */
#define _2JUN_DMDIR_(DTyp) { if   ( _Data_Ptr_  != NULL)                        \
                               { JUN_DP  = _Data_Ptr_;                          \
                                 _Arrp_  = _Data_Ptr_  + sizeof(DTyp);   }      } 
/* JUN_DMIND: direct: JUN_DP = *DataPtr                                        */
#define _2JUN_DMIND_(DTyp) { if   ( _Data_Ptr_  != NULL)                        \
                             { if ( _Data_IPtr_ != NULL)                        \
                               { JUN_DP  = _Data_IPtr_;                         \
                                 _Arrp_  = _Data_IPtr_ + sizeof(DTyp); } }      }
//------------------------------------------------------------------------------------------------- 
// Submacro of JUN_START(...) dependend on OptMo:
// Set Number of functionspecific and optional Aegumente in _ArgCnt_ 
//------------------------------------------------------------------------------------------------- 
#define _1JUN_AMTRANS_(mult) /* Set by Functionparameter ArgCnt                  */        \
        { if      (_ArgCnt_ <  JUN_UNDEF) { _OptMo_ = JUN_AMTRANS;                         \
                                           _ArgCnt_ =       mult * _ArgCnt_ +  _fixed_; }  \
          else if (_ArgCnt_ == JUN_UNLIM) { _OptMo_ = JUN_AMUNLIM; _ArgCnt_  =  0xFFFF; }  \
          else                            { _OptMo_ = JUN_AMUNDEF; _ArgCnt_  = _fixed_; }  }                    
/* undefined */
#define _1JUN_AMUNDEF_(mult)              { _OptMo_ = JUN_AMUNDEF; _ArgCnt_  = _fixed_; }
/* unlimted  */
#define _1JUN_AMUNLIM_(mult)              { _OptMo_ = JUN_AMUNLIM; _ArgCnt_  =  0xFFFF; }
//------------------------------------------------------------------------------------------------- 
// JUN_START(...): Head of a JUN-Funktion
//------------------------------------------------------------------------------------------------- 
#define JUN_START(Name,DtaMo,DTyp,DMTy,RTyp,OptMo,ACMu,FixCnt)                                  \
    JUN_RETTYP Name(void *const _Data_Ptr_, uint32_t JUN_DIM, uint16_t _ArgCnt_,...)            \
   { va_list _LVF_Org_;               /* Variadic Argumentlist for Restauration              */ \
     va_list _LVF_Arg_;               /* Variadic Argumentlist for sequential Access         */ \
     va_list _LVF_Rnd_;               /* Variadic Argumentlist for random     Access         */ \
 JUN_RETTYP  JUN_RET ={.UQ={0x0UL,0x0UL}};   /* direct Returnvalue (init with all 0)         */ \
         _1JUN_DM##DtaMo##_(DTyp,DMTy,RTyp); /* Def. of JUN_DP,JUN_RP,_Arrp_ depend. of DtaMo*/ \
      void* _P_;                      /* Pointer for Access on variadic Pointer-Arguments    */ \
    int32_t _ArgIdx_  = 1;            /* Variadic-Arg-Index counting  1 to ArgCnt then 0     */ \
   uint32_t _HlpVar_;                 /* auxilary Variable                                   */ \
   uint16_t _fixed_   = FixCnt;       /* Number of functionspecific Arguments                */ \
   uint16_t _OptMo_   = 0;            /* Mode of enumbering optional Arguments               */ \
static uint16_t _Recurse_ = 0;             /* Counts call of this Function by itself         */ \
const  uint16_t _DtaMo_   = JUN_DM##DtaMo; /* Data-Transfer-Mode of DataPtr (to JUN_END)     */ \
  /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/ \
     { _Recurse_++;                       /* increase Recursioncounter                       */ \
       _2JUN_DM##DtaMo##_(DTyp);          /* init JUN_DP und _Arrp_                          */ \
       _1JUN_AM##OptMo##_(ACMu);          /* Set Number of func.spec and optional Arg.       */ \
       if (_ArgCnt_ == 0 ) _ArgIdx_  = 0; /* Handle Nunber of variadic argumnets is zero     */ \
       va_start(_LVF_Org_, _ArgCnt_ );    /* Init. Variadic List                             */ \
       va_copy( _LVF_Arg_, _LVF_Org_);    /* Copy Variadic List Org to Arg                   */ \
       va_copy( _LVF_Rnd_, _LVF_Org_);    /* Init Variadic List Rnd                          */ \
    }                     /* outer closing Bracket "}" isnt missing, its in Macro JUN_END -> */                 
//--------------------------------------------------------------------------------------------------
// JUN_EXIT(): behaves as "return JUN_Ret" caue "return" ist forbidden inside a JUN-Function 
//--------------------------------------------------------------------------------------------------
#define JUN_EXIT()    { goto _FPVU_Func_Exit_; }
//--------------------------------------------------------------------------------------------------
// JUN_END(...): Endcode of a JUN-Function
//------------------------------------------------------------------------------------------------- 
#define JUN_END()  {  _FPVU_Func_Exit_:  /* Label to exit JUN-Function                       */ \
   va_end(_LVF_Rnd_);                    /* End Access variadic List                         */ \
   va_end(_LVF_Arg_);                    /* End Access variadic List                         */ \
   va_end(_LVF_Org_);                    /* End Access variadic List                         */ \
   if (_Data_Ptr_) switch (_DtaMo_)      /* Return Data in *JUN_DP/*JUN_RP by *DataPtr       */ \
   { case JUN_DMIND: if(JUN_RP) { _Data_IPtr_ = JUN_RP; break;} /* DtaMo = indirect: JUN_RP  */ \
                     /* else */   _Data_IPtr_ = JUN_DP; break;  /*               or  JUN_DP  */ \
     case JUN_DMRET:              _Data_IPtr_ = JUN_RP; break;  /* DtaMo = ret.only: JUN_RP  */ \
     default:                                           break;  } /* else no return          */ \
   _Recurse_--;                          /* decrease Recursioncounter                        */ \
   return JUN_RET; }                     /* directer Return Value                            */ \
                              /* outer closing Bracket "}" to Bracket opened in JUN_START -> */ }                                                                         
 
//=================================================================================================
// Macrros to Access the Data-Structur transferred or returned by DaraPtr 
// respechtive JUN_DP,JUN_RP,JUN_ARY,JUN_ARP 
//=================================================================================================
// Getting Properties and Types of Data Transfer
// JUN_GETDM()    : Returns Mode of Data Transfer 
// JUN_DSI(count) : Returns byte-size of Type DTyp includeing an Array of Type DMTy[count]
//                  as last Member of DTyp  
// JUN_TYPD()     : Returns Type DTyp
// JUN_TYPR()     : Returns Type RTyp
// JUN_TYPA()     : Returns Type DMTy 
//------------------------------------------------------------------------------------------------
#define JUN_GETDM()    ((const)_DtaMo_)          
#define JUN_DSI(count) (sizeof(*JUN_DP) + (count) * sizeof(*_Arrp_))    
#define JUN_TYPD typeof(*JUN_DP)
#define JUN_TYPR typeof(*JUN_RP)
#define JUN_TYPA typeof(*_Arrp_)          
//------------------------------------------------------------------------------------------------
// Access Array of Type DMTy and variable size as last Member of DTyp
// JUN_ARP(n) : Pointer to the  n.test Array element
// JUN_ARY(n) :                 n.test Arrayelement
//------------------------------------------------------------------------------------------------
#define JUN_ARP(index)   ( _Arrp_ +index)
#define JUN_ARY(index) (*( _Arrp_ +index))
//------------------------------------------------------------------------------------------------
// JUN_DARE(): Reset JUN_DP to value:
// 1) if Data-Transfer-Mode  direkt (JUN_DMDIR): Function Argument  DataPtr
// 2) if Data-Transfer-Mode indiekt (JUN_DMIND): Function Argument *DataPtr resp. last
//                                               Adress set by JUN_DSET() / JUN_RALL() 
//------------------------------------------------------------------------------------------------
#define JUN_DARE()                                                             \
   { switch(_DtaMo_)                                                           \
    { case JUN_DMIND: if (_Data_Ptr_) { JUN_DP = _Data_IPtr_; break; }         \
      case JUN_DMDIR:                   JUN_DP = _Data_Ptr_;  break;           \
      default       :                                         break;   }       }
//------------------------------------------------------------------------------------------------
// JUN_DSET(): Resistant set JUN_DP to other (Variable)Adresse 
// - JUN_ARP() / JUN_ARY() now will refere to this Adress
// - from now JUN_DARE() will set JUN_DP() back to this Adresse  
// - JUN_DIM = count
//------------------------------------------------------------------------------------------------          
#define JUN_DSET(adr,count) { switch(_DtaMo_)       /* dependent on _DtaMo_        */ \
   { case JUN_DMIND:                                /* _DtaMo_ == JUN_DMIND_       */ \
          _Data_IPtr_ = JUN_DP = adr;               /* set JUN_DP                  */ \
          _Arrp_ =((void*)JUN_DP) +sizeof(*JUN_DP); /* correct JUN_ARY/JUN_ARP     */ \
          JUN_DIM= count; break;                    /* set Number of Array Items   */ \
     case JUN_DMDIR:                                /* _DtaMo_ == JUN_DMDIR_       */ \
          JUN_DP = adr;                             /* set JUN_DP                  */ \
          _Arrp_ =((void*)JUN_DP) +sizeof(*JUN_DP); /* correct JUN_ARY/JUN_ARP     */ \
          JUN_DIM= count; break;                    /* set Number of Array Items   */ \
      default: break; }                             /* else: no effect             */ } 

//------------------------------------------------------------------------------------------------
// JUN_RALL(): If Data-Structur given by DataPtr was allocated dynmaically , 
//             change Size of variable-size array of Type DMTy beeing the last
//             Member of Dtyp. 
//             changes JUN_DP, JUN_ARP/ARY(), JUN_DIM and errno 
//------------------------------------------------------------------------------------------------          
#define JUN_RALL(count) { if (_DtaMo_ == JUN_DMIND) /* only in JUN_DMIND            */ \
   { errno = 0;                                     /* init: no Error               */ \
     _HlpVar_ = count;                              /* necassary if ++ in Macro arg.*/ \
     JUN_DP = realloc(JUN_DP,JUN_DSI(_HlpVar_));    /* Returnvalue NULL at Error    */ \
     if (JUN_DP == NULL)                            /* On Error restore JUN_DP      */ \
     { if (_Data_Ptr_) JUN_DP= _Data_IPtr_;         /* Restore JUN_DP               */ \
       errno = ENOMEM;                              /* Set Errno                    */ \
     } else                                                                            \
     { _Data_IPtr_ = JUN_DP;                        /* make change resitant to      */ \
                                                    /* JUN_DARE()                   */ \
       _Arrp_ =((void*)JUN_DP) +sizeof(*JUN_DP);    /* correct JUN_ARY/JUN_ARP      */ \
       JUN_DIM= _HlpVar_;                      }}   /* set Number of Array Items    */ }

//=================================================================================================
// Macros to access optional and functionsspecific Arguments 
//=================================================================================================
// Get Informatione about optional and func.specific Aguments
// JUN_GETNAM()  : Get emumbering-mode of optional Arguments 
// JUN_ARGCNT()  : Get Number of usable func.spec. and opt. Arguments (Sum of both)
// JUN_FIXED()   : Get Number of usable func.spec Arguments
// JUN_ARGIDX()  : Get Index of next func.spec./opt. Arument for sequentiell reading
// JUN_EOVA(Var) : For use in Loop-Condition to process all func.spec and opt Arg.   
//--------------------------------------------------------------------------------------------------
#define JUN_GETAM()   ((const) _OptMo_)
#define JUN_ARGCNT()  ((const)_ArgCnt_)
#define JUN_FIXED()   ((const) _fixed_)    
#define JUN_ARGIDX()  (_ArgIdx_ -1)
#define JUN_EOVA(Var) (  Var=(_ArgIdx_) ? (_ArgIdx_-1): Var , (_ArgIdx_ != 0)  ) 

//--------------------------------------------------------------------------------------------------
// sequential Reading next func.spec/optional Argument
// JUN_GET(   Typ,Ersatz) : Read next Argument
// JUN_PTRGET(Typ,Ersatz) : Read value the next Argument (whitch is assumed to be a 
//                          Pointer of Type Typ*) points to
// - Typ:      Type of Argument to be read
// - Replace: Replacment Value given back if reading isent possible
//--------------------------------------------------------------------------------------------------
// Submacro von JUN_GET und JUN_PTRGET: Argumentindex weitersetzen
// Trick: Multiplikation mit Bedingung: 1 wenn bedingung Wahr, 0 wenn Bedingung Falsch ist
#define _JUN_NEXT_               (_ArgIdx_ = (_ArgIdx_ < _ArgCnt_) * (_ArgIdx_+1))

#define JUN_GET(   Typ,Replace) ( (_ArgIdx_ != 0) ? (_JUN_NEXT_,       va_arg(_LVF_Arg_,Typ ))  \
                                                  : Replace                                     )

#define JUN_PTRGET(Typ,Replace) ( (_ArgIdx_ != 0) ? (_JUN_NEXT_, _P_ = va_arg(_LVF_Arg_,void*), \
                                                    (_P_ != NULL)? *((Typ*)(_P_)) : Replace  )  \
                                                  : Replace                                     )

//--------------------------------------------------------------------------------------------------
// JUN_RNDGET: Read functionspecific or optionale Argument by Index (Random access) 
//             The Argument Index for sequential reading will not be changed.  
//
// !!! ATTENTION !!! The correkt Behaviour of this Macros depends on a property
//                   of the Macro va_arg() (Header stdarg.h) whitch is not guaranteed by the
//                   Standart C Library. So it might not work on all systems !
//
// Var   : Variable for reading in func.spec/opt Argument
// Nr    : Index of func.spec/opt Argument ro be readed
// Ref   : "" (emty) for normal Behaviour / "P" to read Value the Pointer in the func.spec/opt
//                                              Argument points to
// Replace: Replacment Value given back if reading isent possible
//--------------------------------------------------------------------------------------------------
// submacro JUN_RNDGET(): Operation if Ref==""
#define _JUN_RNDGET_(Var,Nr,Replace) {                                                           \
   if ( Nr  <= _ArgCnt_  )                     /* Test weather Index of Argument is valid     */ \
   { va_copy( _LVF_Rnd_, _LVF_Org_);           /* Create working copy of Variadic List        */ \
     for (_HlpVar_=0; _HlpVar_<Nr; _HlpVar_++) /* Step over Nr. -1 Arguments                  */ \
     {     va_arg(_LVF_Rnd_,typeof(Var)); }                                                      \
     Var = va_arg(_LVF_Rnd_,typeof(Var));      /* Read Argument Number Nr.                    */ \
   } else { Var = Replace; }                   /* else return Replacement Value               */ }

// submacro JUN_RNDGET(): Operation if fuer Ref=="P" 
#define _JUN_RNDGETP_(Var,Nr,Replace) {                                                          \
   if ( Nr  <= _ArgCnt_  )                     /* Test weather Index of Argument is valid     */ \
   { va_copy( _LVF_Rnd_, _LVF_Org_);           /* Create working copy of Variadic List        */ \
     for (_HlpVar_=0; _HlpVar_<Nr; _HlpVar_++) /* Step over Nr. -1 Arguments                  */ \
     {      va_arg(_LVF_Rnd_,void*);  }                                                          \
      _P_ = va_arg(_LVF_Rnd_,void*);                /* Read Argument (Pointer) Number Nr.     */ \
      Var = (_P_ != NULL)? *( (typeof(Var)*)(_P_) ) /* dereference this Pointer               */ \
                         : Replace;                 /* if NULL return Replacement Value       */ \
   } else { Var = Replace; }                        /* else return Replacement Value          */ }

// Main Macro JUN_RNDGET
#define JUN_RNDGET(Var,Nr,Ref,Replace) _JUN_RNDGET##Ref##_(Var,Nr,Replace)

//--------------------------------------------------------------------------------------------------
// JUN_ADDOPT(Cnt): Add Macroparameter Cnt to number of optional Arguments -> JUN_ARGCNT()
//           ( especially used to set Number of optional Arguments in case of  
//             Enumbering-Mode OptMo==UNDEF )
//--------------------------------------------------------------------------------------------------
#define JUN_ADDOPT(Cnt) {                                                                     \
   if ( (_OptMo_ != JUN_AMUNLIM ) && (Cnt >=0) )  /* No Effect for OptMo == UNLIM          */ \
   { _ArgCnt_ += (Cnt);           /* increase count of opt. Args by Cnt                    */ \
     _OptMo_   = JUN_AMTRANS;     /* enumbering-mode: Cnt of opt. Arg is now status set    */ \
     /* if ArgIdx indicates End of list, set it to first added Arg                         */ \
     if ( ( _ArgIdx_ == 0) && (_ArgCnt_ !=0) ) { _ArgIdx_ = _ArgCnt_; }  }                    }

//--------------------------------------------------------------------------------------------------
// JUN_SHORTEN() : Shorten Argument list to actual Position of sequential reading
//--------------------------------------------------------------------------------------------------
#define JUN_SHORTEN() {                                                                       \
   va_copy(_LVF_Org_,_LVF_Arg_); /* copying actual List _LVF_Arg_ to orginal list          */ \
                                 /* _LVF_Org_ means shortening _LVF_Org                    */ \
   if (_ArgIdx_ > 0)             /* do not shorten to empty list                           */ \
   { if (_OptMo_ != JUN_AMUNLIM)                                                              \
     { _ArgCnt_ -= (_ArgIdx_-1)  ; } /* OptMo != UNLIM: ajust nunbmer of Args              */ \
     else { _ArgCnt_  = JUN_UNLIM; } /* OptMO == UNLIM: always unlimited                   */ \
     if (_fixed_ >= _ArgIdx_) _fixed_ -= (_ArgIdx_-1); /* decrease number of func.spec Args*/ \
     else                     _fixed_  = 0;            /* ... but dont get negative        */ \
     _ArgIdx_ = 1;                                     /* reset Argument Index             */ \
    } else                                                                                    \
    { if (_OptMo_ != JUN_AMUNLIM) { _ArgCnt_=0; }   /* empty List: Cnt.     opt. Args. = 0 */ \
      _fixed_ = 0;                                } /* empty List: Cnt fkt.spec. Args. =0  */ }

//--------------------------------------------------------------------------------------------------
// JUN_RES() : Rest Argument-List to first Argument (for re-reading)
//-------------------------------------------------------------------------------------------------- 
#define JUN_RES() { if (_ArgCnt_ > 0) {va_copy( _LVF_Arg_, _LVF_Org_);  _ArgIdx_ = 1;} \
                    else                                                _ArgIdx_ = 0;  }

//--------------------------------------------------------------------------------------------------
// JUN_DUP(Mode,Variadic) : copy Argument List to Variadic List (of Type va_list) for use by
//                          Macros in Header stdarg.h 
// Mode == 0: copy Argument List at Position of seq. reading 
// Mode != 0: copy Argument Liste (Original) from first Position
//-------------------------------------------------------------------------------------------------- 
#define JUN_DUP(Mode,Variadic) { if(Mode) va_copy( Variadic, _LVF_Org_); \
                                 else     va_copy( Variadic, _LVF_Arg_); }

//=================================================================================================
// further Macros 
//=================================================================================================
// JUN_RECURSE: read recursion counter of this JUN-Funktion (counts calling depth)
//-------------------------------------------------------------------------------------------------- 
#define JUN_RECURSE() (((const)_Recurse_)-1) 

//=================================================================================================
// Macros for calling a JUN-Function
//=================================================================================================
// EXE_JUN(   FuncPtr,Member,DataPtr,ArDim,ArgCnt,...) : Calling with    Datatransfer by DataPtr
// EXE_JUN_ND(FuncPtr,Member,              ArgCnt,...) : Calling without Datatransfer by DataPtr
//
// FuncPtr:    Pointer to JUN-Function to be called (Adress)
// Member:     Member of direct Returnvalue JUN_RETTYP to be returnd. 
//             ( empty is whole JUN_RETTYP is to be returned)
// DataPtr:    DtaMo = DMDIR       : Pointer            to Data to be transfered
//             DtaMo = DMIND/DMRET : Pointer to Pointer to Data to be transfered 
// ArDim:      Size (in items) of Variable-Size Array beeing the last Member of Data Structur
// ArgCnt:     Number of optional Arguments(Groups)
// [Variadic]: functionspecific Arguments
// [Variadic]: optional Arguments     
//-------------------------------------------------------------------------------------------------
#define EXE_JUN(   FuncPtr,Member,   DataPtr  ,ArDim,ArgCnt,...          )         \
              (( (*FuncPtr) ((void*)(&DataPtr),ArDim,ArgCnt,__VA_ARGS__) )Member)

#define EXE_JUN_ND(FuncPtr,Member,                   ArgCnt,...          )         \
              (( (*FuncPtr) (NULL             ,0    ,ArgCnt,__VA_ARGS__) )Member)

//*************************************************************************************************
#endif // to #ifndef _JUN_h_def_ 