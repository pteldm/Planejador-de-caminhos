#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>

#include "planejador.h"

using namespace std;

/* *************************
   * CLASSE IDPONTO        *
   ************************* */

/// Atribuicao de string
void IDPonto::set(string&& S)
{
  t=move(S);
  if (!valid()) t.clear();
}

/* *************************
   * CLASSE IDROTA         *
   ************************* */

/// Atribuicao de string
void IDRota::set(string&& S)
{
  t=move(S);
  if (!valid()) t.clear();
}

/* *************************
   * CLASSE PONTO          *
   ************************* */

/// Distancia entre 2 pontos (formula de haversine)
double haversine(const Ponto& P1, const Ponto& P2)
{
  // Tratar logo pontos identicos
  if (P1.id == P2.id) return 0.0;

  static const double MY_PI = 3.14159265358979323846;
  static const double R_EARTH = 6371.0;
  // Conversao para radianos
  double lat1 = MY_PI*P1.latitude/180.0;
  double lat2 = MY_PI*P2.latitude/180.0;
  double lon1 = MY_PI*P1.longitude/180.0;
  double lon2 = MY_PI*P2.longitude/180.0;

  double cosseno = sin(lat1)*sin(lat2) + cos(lat1)*cos(lat2)*cos(lon1-lon2);
  // Para evitar eventuais erros na funcao acos por imprecisao numerica
  // nas operacoes com double: acos(1.0000001) eh NAN
  if ( cosseno > 1.0 ) cosseno = 1.0;
  if ( cosseno < -1.0 ) cosseno = -1.0;
  // Distancia entre os pontos
  return R_EARTH*acos(cosseno);
}

/* *************************
   * CLASSE PLANEJADOR     *
   ************************* */

/// Torna o mapa vazio
void Planejador::clear()
{
  pontos.clear();
  rotas.clear();
}

/// Retorna um Ponto do mapa, passando a id como parametro.
/// Se a id for inexistente, retorna um Ponto vazio.
Ponto Planejador::getPonto(const IDPonto& Id) const
{
  // Procura um ponto que corresponde aa Id do parametro
  /* ***********  /
  /  FALTA FAZER  /
  /  *********** */
  // Em caso de sucesso, retorna o ponto encontrado
  /* ***********  /
  /  FALTA FAZER  /
  /  *********** */
  // Se nao encontrou, retorna um ponto vazio
  /// FEITO
  auto itr = find(pontos.begin(),pontos.end(), Id);
  if(itr!= pontos.end()) return *itr;

  return Ponto();
}

/// Retorna um Rota do mapa, passando a id como parametro.
/// Se a id for inexistente, retorna um Rota vazio.
Rota Planejador::getRota(const IDRota& Id) const
{
  // Procura uma rota que corresponde aa Id do parametro
  /* ***********  /
  /  FALTA FAZER  /
  /  *********** */
  // Em caso de sucesso, retorna a rota encontrada
  /* ***********  /
  /  FALTA FAZER  /
  /  *********** */
  // Se nao encontrou, retorna uma rota vazia
  ///FEITO
  auto itr = find(rotas.begin(), rotas.end(), Id);
  if(itr!=rotas.end()) return *itr;

  return Rota();
}

/// Imprime os pontos do mapa no console
void Planejador::imprimirPontos() const
{
  for (const auto& P : pontos)
  {
    cout << P.id << '\t' << P.nome
         << " (" <<P.latitude << ',' << P.longitude << ")\n";
  }
}

/// Imprime as rotas do mapa no console
void Planejador::imprimirRotas() const
{
  for (const auto& R : rotas)
  {
    cout << R.id << '\t' << R.nome << '\t' << R.comprimento << "km"
         << " [" << R.extremidade[0] << ',' << R.extremidade[1] << "]\n";
  }
}

bool Planejador::getUniquePonto( const std::list<Ponto>& lista, const Ponto& P) const
{
    auto itr = find(lista.begin(),lista.end(),P.id);
    if( *itr == P.id ) return true;

    return false;
}

bool Planejador::getUniqueRota( const std::list<Rota>& lista, const Rota& R) const
{
    auto itr = find(lista.begin(), lista.end(),R.id);
    if(*itr == R.id ) return true;

    return false;
}

bool Planejador::verifyID( const std::list<Ponto>& lista, const IDPonto& extremidade)
{
    auto itr = find(lista.begin(), lista.end(), extremidade);
    if(*itr == extremidade) return true;

    return false;
}

/// Leh um mapa dos arquivos arq_pontos e arq_rotas.
/// Caso nao consiga ler dos arquivos, deixa o mapa inalterado e retorna false.
/// Retorna true em caso de leitura bem sucedida
bool Planejador::ler(const std::string& arq_pontos,
                     const std::string& arq_rotas)
{
  // Listas temporarias para armazenamento dos dados lidos
  list<Ponto> listP;
  list<Rota> listR;
  // Variaveis auxiliares para buscas nas listas
  list<Ponto>::iterator itr_ponto;
  list<Rota>::iterator itr_rota;
  // Variaveis auxiliares para leitura de dados
  Ponto P;
  Rota R;
  string prov;

  // Leh os pontos do arquivo
  try
  {
    // Abre o arquivo de pontos
    ifstream arq(arq_pontos);
    if (!arq.is_open()) throw 1;

    // Leh o cabecalho
    getline(arq,prov);
    if (arq.fail() ||
        prov != "ID;Nome;Latitude;Longitude") throw 2;

    // Leh os pontos
    do
    {
      // Leh a ID
      getline(arq,prov,';');
      if (arq.fail()) throw 3;
      P.id.set(move(prov));
      if (!P.valid()) throw 4;

      // Leh o nome
      getline(arq,prov,';');
      if (arq.fail() || prov.size()<2) throw 5;
      P.nome = move(prov);

      // Leh a latitude
      arq >> P.latitude;
      if (arq.fail()) throw 6;
      arq.ignore(1,';');

      // Leh a longitude
      arq >> P.longitude;
      if (arq.fail()) throw 7;
      arq >> ws;

      // Verifica se jah existe ponto com a mesma ID no conteiner de pontos lidos (listP)
      // Caso exista, throw 8
      /* ***********  /
      /  FALTA FAZER  /
      /  *********** */
      /// FEITO
      if(getUniquePonto(listP, P)) throw 8;

      // Inclui o ponto na lista de pontos
      listP.push_back(move(P));
    }
    while (!arq.eof());

    // Fecha o arquivo de pontos
    arq.close();
  }
  catch (int i)
  {
    cerr << "Erro " << i << " na leitura do arquivo de pontos "
         << arq_pontos << endl;
    return false;
  }

  // Leh as rotas do arquivo
  try
  {
    // Abre o arquivo de rotas
    ifstream arq(arq_rotas);
    if (!arq.is_open()) throw 1;

    // Leh o cabecalho
    getline(arq,prov);
    if (arq.fail() ||
        prov != "ID;Nome;Extremidade 1;Extremidade 2;Comprimento") throw 2;

    // Leh as rotas
    do
    {
      // Leh a ID
      getline(arq,prov,';');
      if (arq.fail()) throw 3;
      R.id.set(move(prov));
      if (!R.valid()) throw 4;

      // Leh o nome
      getline(arq,prov,';');
      if (arq.fail() || prov.size()<2) throw 4;
      R.nome = move(prov);

      // Leh a id da extremidade[0]
      getline(arq,prov,';');
      if (arq.fail()) throw 6;
      R.extremidade[0].set(move(prov));
      if (!R.extremidade[0].valid()) throw 7;

      // Verifica se a Id corresponde a um ponto no conteiner de pontos lidos (listP)
      // Caso ponto nao exista, throw 8
      /* ***********  /
      /  FALTA FAZER  /
      /  *********** */
      /// FEITO
      if(!verifyID(listP, R.extremidade[0])) throw 8;
      // Leh a id da extremidade[1]

      getline(arq,prov,';');
      if (arq.fail()) throw 9;
      R.extremidade[1].set(move(prov));
      if (!R.extremidade[1].valid()) throw 10;

      // Verifica se a Id corresponde a um ponto no conteiner de pontos lidos (listP)
      // Caso ponto nao exista, throw 11
      /* ***********  /
      /  FALTA FAZER  /
      /  *********** */
      /// FEITO
      if(!verifyID(listP, R.extremidade[1])) throw 11;

      // Leh o comprimento
      arq >> R.comprimento;
      if (arq.fail()) throw 12;
      arq >> ws;

      // Verifica se jah existe rota com a mesma ID no conteiner de rotas lidas (listR)
      // Caso exista, throw 13
      /* ***********  /
      /  FALTA FAZER  /
      /  *********** */

      // Inclui a rota na lista de rotas
      listR.push_back(move(R));
    }
    while (!arq.eof());

    // Fecha o arquivo de rotas
    arq.close();
  }
  catch (int i)
  {
    cerr << "Erro " << i << " na leitura do arquivo de rotas "
         << arq_rotas << endl;
    return false;
  }

  // Soh chega aqui se nao entrou no catch, jah que ele termina com return.
  // Move as listas de pontos e rotas para o planejador.
  pontos = move(listP);
  rotas = move(listR);

  return true;
}

/// *******************************************************************************
/// Calcula o caminho entre a origem e o destino do planejador usando o algoritmo A*
/// *******************************************************************************

/// Noh: os elementos dos conjuntos de busca do algoritmo A*
/* ***********  /
/  FALTA FAZER  /
/  *********** */

/// Calcula o caminho entre a origem e o destino do planejador usando o algoritmo A*
/// Retorna o comprimento do caminho encontrado.
/// (<0 se  parametros invalidos ou nao existe caminho).
/// O parametro C retorna o caminho encontrado
/// (vazio se  parametros invalidos ou nao existe caminho).
/// O parametro NA retorna o numero de nos em aberto ao termino do algoritmo A*
/// (<0 se parametros invalidos, retorna >0 mesmo quando nao existe caminho).
/// O parametro NF retorna o numero de nos em fechado ao termino do algoritmo A*
/// (<0 se parametros invalidos, retorna >0 mesmo quando nao existe caminho).
double Planejador::calculaCaminho(const IDPonto& id_origem,
                                  const IDPonto& id_destino,
                                  Caminho& C, int& NA, int& NF)
{
  // Zera o caminho resultado
  C.clear();

  try
  {
    // Mapa vazio
    if (empty()) throw 1;

    // Calcula o ponto que corresponde a id_origem.
    // Se nao existir, throw 4
    Ponto pt_orig = getPonto(id_origem);
    if (!pt_orig.valid()) throw 4;

    // Calcula o ponto que corresponde a id_destino.
    // Se nao existir, throw 5
    Ponto pt_dest = getPonto(id_destino);
    if (!pt_dest.valid()) throw 5;

    /* *****************************  /
    /  IMPLEMENTACAO DO ALGORITMO A*  /
    /  ***************************** */
    /* ***********  /
    /  FALTA FAZER  /
    /  *********** */

    // O try tem que terminar retornando o comprimento calculado
    return -1.0;  // SUBSTITUA pelo return do valor correto
  }
  catch(int i)
  {
    cerr << "Erro " << i << " no calculo do caminho\n";
  }

  // Soh chega aqui se executou o catch, jah que o try termina sempre com return.
  // Caminho C permanece vazio.
  NA = NF = -1;
  return -1.0;
}
