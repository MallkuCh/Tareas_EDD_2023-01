#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Retorna true si ambos strings son iguales */
bool string_equals(char *string1, char *string2) {
  return !strcmp(string1, string2);
}

/* Revisa que los parametros del programa sean válidos */
bool check_arguments(int argc, char **argv) {
  if (argc != 3) {
    printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
    printf("Donde:dasd\n");
    printf("\tINPUT es la ruta del archivo de input\n");
    printf("\tOUTPUT es la ruta del archivo de output\n");
    return false;
  }
  return true;
}

int main(int argc, char **argv) {
  /* Si los parámetros del programa son inválidos */

  if (!check_arguments(argc, argv)) {
    /* Salimos del programa indicando que no terminó correctamente */
    return 1;
  }

  /* Abrimos el archivo de input */
  FILE *input_file = fopen(argv[1], "r");

  /* Abrimos el archivo de output */
  FILE *output_file = fopen(argv[2], "w");

  /* Leemos la cantidad de candidatos y de eventos */
  int N_CANDIDATES;
  fscanf(input_file, "%d", &N_CANDIDATES);

  /* estructura para almacenar los candidatos */

  typedef struct vote{
    int id_vote;
    int id_candidate;
    struct vote *next;
  } vote;

  typedef struct candidate
  {
    int id_candidate;
    int tomar_distancia; /*1 = SI; 0 = NO*/
    vote *first_vote;
    vote *last_vote;
  } candidate;

  typedef struct sort_candidate
  {
    int cantidad_votos;
    int id_candidate;
    struct sort_candidate *next;
  } sort_candidate;

  candidate* create_candidate(int id_candidate){
    candidate* candidate = calloc(1, sizeof(candidate));
    candidate->id_candidate=id_candidate;
    candidate->tomar_distancia = 1;
    return candidate;
  }

  vote* create_vote(int id_vote, int id_candidate){
    vote* vote = calloc(1, sizeof(vote));
    vote->id_vote=id_vote;
    vote->id_candidate=id_candidate;
    return vote;
  }

  sort_candidate* create_sort_candidate(int cantidad_votos, int id_candidate){
    struct sort_candidate* sort_candidate = calloc(1, sizeof(sort_candidate));
    sort_candidate->cantidad_votos = cantidad_votos;
    sort_candidate->id_candidate = id_candidate;
    sort_candidate->next = NULL;
    return sort_candidate;
  }

  void insertvote(candidate* candidate, vote* vote) {
    if (candidate->first_vote == NULL) {
      candidate->first_vote=vote;
    } else {
      candidate->last_vote->next=vote;
    }
    candidate->last_vote = vote;
  }

  void freeCandidate(candidate* candidate){
    vote* vote = candidate->first_vote;

    while( vote !=NULL )
    {
      struct vote* next_vote = vote->next;
      vote = next_vote;
    }

    free(candidate);
  }

/* Creacion total de candidatos */
  candidate** candidates = calloc(N_CANDIDATES, sizeof(candidate**));
  for (int i = 0; i < N_CANDIDATES; i++) {
    candidates[i] = create_candidate(i);
  }

void intercambio_candidate(struct sort_candidate *sorted_1, struct sort_candidate *sorted_2)
{
  int temp = sorted_1->cantidad_votos;
  int temp_2 = sorted_1->id_candidate;
  sorted_1->cantidad_votos = sorted_2->cantidad_votos;
  sorted_1->id_candidate = sorted_2->id_candidate;
  sorted_2->cantidad_votos = temp;
  sorted_2->id_candidate = temp_2;
}

void ordenar_candidatos(struct sort_candidate *inicio){
  int intercambio;
  struct sort_candidate *sort_candidate_actual;
  struct sort_candidate *sort_candidate_anterior = NULL;

  if (inicio == NULL)
  {
    return;
  }

  do{
    intercambio = 0;
    sort_candidate_actual = inicio;

    while (sort_candidate_actual->next != sort_candidate_anterior)
    {
      if (sort_candidate_actual->cantidad_votos < sort_candidate_actual->next->cantidad_votos)
      {
        intercambio_candidate(sort_candidate_actual, sort_candidate_actual->next);
        intercambio = 1;
      }
      else if (sort_candidate_actual->cantidad_votos == sort_candidate_actual->next->cantidad_votos)
      {
        if (sort_candidate_actual->id_candidate > sort_candidate_actual->next->id_candidate)
        {
          printf(output_file, "%d ,  %d", sort_candidate_actual->id_candidate, sort_candidate_actual->next->id_candidate);
          intercambio_candidate(sort_candidate_actual, sort_candidate_actual->next);
          intercambio = 1;
        }

       /* else if(sort_candidate_actual->id_candidate < sort_candidate_actual->next->id_candidate){
          intercambio_candidate(sort_candidate_actual->next, sort_candidate_actual);
          intercambio = 1;
        }*/
      }
      sort_candidate_actual = sort_candidate_actual->next;
    }
    sort_candidate_anterior =sort_candidate_actual;
  } while (intercambio);
}

  int N_EVENTS;
  fscanf(input_file, "%d", &N_EVENTS);

  /* String para guardar la instrucción actual*/
  char command[32];

  for (int event = 0; event < N_EVENTS; event++) {
    /* Leemos el evento */
    fscanf(input_file, "%s", command);

    /* [Por implementar] A continuación debes implementar cada evento */
    if (string_equals(command, "REGISTRAR-VOTO")) {

      int voter_ID, candidate_ID;
      fscanf(input_file, "%d %d", &voter_ID, &candidate_ID);
      vote* vote_actual = create_vote(voter_ID, candidate_ID);
      insertvote(candidates[(candidate_ID)], vote_actual);
      fprintf(output_file, "VOTO REGISTRADO %d\n", voter_ID);
    }
    else if (string_equals(command, "CONTEO-PARCIAL")) {
      /* [Por implementar] */
      char var = "CONTEO-PARCIAL\n";
      fprintf(output_file,  "CONTEO-PARCIAL\n");
      int conteo_final = 0;

      for (int i = 0; i < N_CANDIDATES; i++) {
        candidate* candidate = candidates[i];
        vote* vote = candidate->first_vote;
        int conteo = 0;
        char var = "        CANDIDATE %d\n";
        fprintf(output_file,"        CANDIDATE %d\n", candidate->id_candidate);
        if (vote == NULL)
        {
          char var ="                  NO HAY VOTOS REGISTRADOS\n";
          fprintf(output_file,  "             NO HAY VOTOS REGISTRADOS\n");
        }
        else
        {
          while (vote != NULL){
            char var = "                VOTE %d\n";
            fprintf(output_file, "                VOTE %d\n" ,vote->id_vote);
            conteo=conteo+1;
            vote = vote->next;

        }
        }
      conteo_final = conteo + conteo_final;
      }
      fprintf(output_file, "TOTAL PARCIAL DE VOTOS: %d\n", conteo_final);
    }
    else if (string_equals(command, "CONTEO-TOTAL")) {
      /* [Por implementar] */
      fprintf(output_file,"CONTEO-TOTAL\n" );
      int conteo_final = 0;
      for (int i = 0; i < N_CANDIDATES; i++) {
        candidate* candidate = candidates[i];
        vote* vote = candidate->first_vote;

        int conteo = 0;
        while (vote != NULL)
        {
          conteo++;
          vote = vote->next;
        }
        conteo_final = conteo + conteo_final;
        fprintf(output_file, "        CANDIDATO %d: %d\n", candidate -> id_candidate, conteo);
      }
      fprintf(output_file, "TOTAL VOTOS: %d\n", conteo_final);
    }
    else if (string_equals(command, "CONTEO-RANGO")) {
      int min_votes, max_votes;
      fscanf(input_file, "%d %d", &min_votes, &max_votes);
      /* [Por implementar] */
      fprintf(output_file, "CONTEO RANGO %d %d\n", min_votes, max_votes);
      int conteo_final = 0;
      for (int i = 0; i < N_CANDIDATES; i++){
        candidate* candidate = candidates[i];
        vote* vote = candidate->first_vote;

        int conteo = 0;
        while (vote != NULL)
        {
          conteo=conteo+1;
          vote = vote->next;
        }

      if (min_votes <= conteo)
      {
        if (conteo <= max_votes)
        {
          conteo_final = conteo + conteo_final;
          fprintf(output_file, "CANDIDATO %d: %d\n", candidate -> id_candidate, conteo);
        }
      }
      }
      fprintf(output_file, "TOTAL DE VOTOS RANGO: %d\n" , conteo_final);


    }
    else if (string_equals(command, "ORDENAR-CANDIDATOS")) { /*  ORDENAR CANDIADTOS            */
      /* [Por implementar] */
      /* Funcion de contado total y en base a eso hacer una lista ligada de de votos totales e id_candidato */
      /* Para esto se va a generar una estrucutra extra de votos_por_candidato */
      int conteo_final = 0;
      struct sort_candidate *inicio = NULL;
      struct sort_candidate *actual;

      for (int i = 0; i < N_CANDIDATES; i++){
        candidate* candidate = candidates[i];
        vote* vote = candidate->first_vote;

        int conteo = 0;
        while (vote != NULL)
        {
          conteo++;
          vote = vote->next;
        }
        conteo_final = conteo + conteo_final;
        sort_candidate* nuevo_sort_candidate = create_sort_candidate(conteo, candidate->id_candidate);


        if (inicio == NULL){
          inicio = nuevo_sort_candidate;
          actual = inicio;
        }
        else{
          actual->next = nuevo_sort_candidate;
          actual = actual->next;
        }
      }

      /* Usar un algoritmo sorted selection o sorted by group para ordenar cada cosa*/

      ordenar_candidatos(inicio);

      /*FINALMENTE SE IMPRIMEN LOS CANDIDATOS ORDENADOS*/

      fprintf(output_file, "CANDIDATOS ORDENADOS\n");

      void imprimir(struct sort_candidate *sort_candidate){
        while (sort_candidate != NULL)
        {
          fprintf(output_file, "        CANDIDATO %d %d\n", sort_candidate->id_candidate, sort_candidate->cantidad_votos);
          sort_candidate = sort_candidate->next;
        }
      } /* MODULARIZAR!!!!!!!!!!!!!!!!!*/

      imprimir(inicio);

    }


    else if (string_equals(command, "ANULAR-VOTO")) {
      int voter_ID, candidate_ID;
      fscanf(input_file, "%d %d", &voter_ID, &candidate_ID);
      /* [Por implementar] */

      int buscar_candidate(int candidate_ID,int N_CANDIDATES){
        for (int i = 0; i < N_CANDIDATES; i++)
        {
          if (candidates[i]->id_candidate == candidate_ID)
          {
            return i;
          }
        }
      }
      /* Funcion para eliminar votos*/
      void eliminar_voto(struct vote** vote, int voter_ID){
        struct vote* temporal = *vote;
        struct vote* previo = NULL;
        while(temporal != NULL && temporal->id_vote != voter_ID){
          previo = temporal;
          temporal = temporal->next;
        }

        if (temporal == NULL){
          fprintf(output_file, "NO SE ENCONTRO UN VOTO VALIDO CON ID %d\n", voter_ID);
          return;
        }

        if (previo == NULL){
          *vote = temporal->next;
          fprintf(output_file, "VOTO ELIMINADO CORRECTAMENTE\n", voter_ID);
        }
        else{
          previo->next = temporal->next;
          fprintf(output_file, "VOTO ELIMINADO CORRECTAMENTE\n", voter_ID);
        }

        free(temporal);
      }

      int id_candidate = buscar_candidate(candidate_ID, N_CANDIDATES);

      candidate* candidate_vote_eliminate = candidates[id_candidate];
      eliminar_voto(&candidate_vote_eliminate->first_vote, voter_ID);

    }
    else if (string_equals(command, "ELIMINAR-CANDIDATO")) {
      /* [Por implementar] */
      int conteo_final = 0;
      struct sort_candidate *inicio = NULL;
      struct sort_candidate *actual;

      for (int i = 0; i < N_CANDIDATES; i++){
        candidate* candidate = candidates[i];
        vote* vote = candidate->first_vote;

        int conteo = 0;
        while (vote != NULL)
        {
          conteo++;
          vote = vote->next;
        }

        conteo_final = conteo + conteo_final;
        sort_candidate* nuevo_sort_candidate = create_sort_candidate(conteo, candidate->id_candidate);

        if (inicio == NULL){
          inicio = nuevo_sort_candidate;
          actual = inicio;
        }
        else{
          actual->next = nuevo_sort_candidate;
          actual = actual->next;
        }
      }

      ordenar_candidatos(inicio);

      int id_ultimo_candidato(struct sort_candidate* sort_candidate) {
        struct sort_candidate* candidato = sort_candidate;
        while (candidato != NULL && candidato->next != NULL && candidato->next->cantidad_votos != 0 && candidato->cantidad_votos != candidato->next->cantidad_votos) {
          candidato = candidato->next;
        }
        int id_ultimo = candidato->id_candidate;
        return id_ultimo;
      }
      int id_eliminar = id_ultimo_candidato(inicio);
      int i;
      int j;

      int posicion_eliminar(int id)
        {
          for (i = 0 ; i <N_CANDIDATES; i++) {
          if (candidates[i]->id_candidate == id)
          {
            return i;
          }
        }
        }

      int id_posicion_eliminar = posicion_eliminar(id_eliminar);
      candidate* candidate_eliminado = candidates[id_posicion_eliminar];
      vote* vote = candidate_eliminado->first_vote;
      candidate_eliminado->first_vote = NULL;
      candidate_eliminado->tomar_distancia = 0;
      int votos_traspasar = 0;

      int min(x,y){
        if (x < y) {
          return x;
        }
        else {
          return y;
        }
      }

      for (i = 0; i < N_CANDIDATES; i++) {
        candidate* candidato_comparacion = candidates[i];
        if (candidato_comparacion->tomar_distancia == 1){
        }
      }

      while (vote != NULL){
        votos_traspasar++;
        vote = vote->next;
      }
      int k,l;
      int menor_distancia = N_CANDIDATES;


    }
    else if (string_equals(command, "TRASPASAR-EXCESO-VOTOS")) {
      int M;
      fscanf(input_file, "%d", &M);
      /* [Por implementar] */
    }
  }

  /*  [Por implementar] Liberamos nuestras estructuras */

  for (int i = 0; i < N_CANDIDATES; i++)
  {
    freeCandidate(candidates[i]);
  }

  free(candidates);

  fclose(input_file);
  fclose(output_file);

  return 0;
}
