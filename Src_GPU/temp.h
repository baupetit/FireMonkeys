/*
 * Auteurs : Nicolas Form & Aupetit Benjamin
 * IUT Aubiere (Clermont-Fd)
 * Description : Fichier d'en-tete contenant toutes les fonctions utiles pour manipuler le temps
 * ( calcul de temps / debit ... )
 *
 */

#ifndef TEMP__H
#define TEMP__H

#include <ctime>

#define NS_SEC 1000000000

#define SET 1
#define GET 0

typedef struct timespec __time_struct;

/*
 * normalise the struct timespec passed as an argument
 */
inline void normalized_timespec( __time_struct *ts ){
  while( ts->tv_nsec >= NS_SEC ){
    (ts->tv_sec)++;
    (ts->tv_nsec)-=NS_SEC;
  }
}


/*
 * return the difference between dsr and rsl ( dsr - rls )
 */
inline __time_struct sub_timespec(__time_struct *dsr , __time_struct *rls){
  __time_struct res;
  res.tv_nsec = dsr->tv_nsec - rls->tv_nsec;
  res.tv_sec = dsr->tv_sec - rls->tv_sec;
  normalized_timespec( &res );
  return res;
}

/*
 * return the time given by clock_gettime with CLOCK_REALTIME
 */
inline __time_struct get_real_time(){
  __time_struct tps;
  clock_gettime( CLOCK_REALTIME , &tps );
  return tps;
}

/*
 * if arg is GET then return a struct timespec with the elapsed time since the last call with SET
 * if arg is SET then set the value of time_set to the current time
 */
__time_struct count_time(char arg){
  static __time_struct time_set = { 0 , 0 };
  __time_struct tmp = { 0 , 0 };
  if( arg ) 
    {
      time_set = get_real_time(); 
    }
  else
    {
      tmp = get_real_time();
      tmp = sub_timespec( &tmp , &time_set );   
    }
  return tmp;
}

/*
 * return the byte rate given by a long ( the total amount of bytes ) and a time
 */
double get_byte_rate( long *byte_amount , __time_struct *tps ){
   
  return (double) *byte_amount / ( tps->tv_sec + ( (double) tps->tv_nsec / NS_SEC ) );
}

/*
 * transform the struct timespec into a double representing a time in seconds
 */
double timespec_to_double( __time_struct *tps ){
  return tps->tv_sec + ( (double)tps->tv_nsec / NS_SEC ) ;
}

#endif
   
