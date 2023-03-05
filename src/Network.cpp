#include "Network.h"
#include "Population.h"
#include "RNG.h"

using namespace Rcpp;

Network::Network(Population &population)
  : Contact(population), _finalized(false)
{
}

Network::~Network()
{
}

const std::vector<PAgent> &Network::contact(double time, Agent &agent)
{
  return _neighbors[agent.id() - 1];
}

void Network::add(const PAgent &agent)
{
  // have we finalized?
  if (_finalized) grow(agent);
}

void Network::finalize()
{
  if (!_finalized) {
    size_t n = _population.size();
    for (size_t i = 0; i < n; ++i)
      _neighbors = std::vector< std::vector<PAgent> >(n);
    build();
    _finalized = true;
  }
}

ConfigurationModel::ConfigurationModel(Population &population, Function degree_rng)
  : Network(population), _rng(degree_rng)
{
}

void ConfigurationModel::build()
{
  IntegerVector d = _rng(_neighbors.size());
  size_t L = sum(d) + 0.5;
  std::vector<int> stubs(L);
  for (size_t i = 0, k = 0; i < d.size(); ++i)
    for (size_t j = 0; j < d[i]; ++j)
      stubs[k++] = i;
  size_t from, to, n = stubs.size();
  while (n > 2) {
    from = RUnif::stdUnif.get() * n;
    to = RUnif::stdUnif.get() * n;
    connect(stubs[from], stubs[to]);
    stubs[from] = stubs[n-1];
    stubs[to] = stubs[n-2];
    n -= 2;
  }
}

void Network::connect(int from, int to)
{
  if (from == to) return;
  // avoid multiple loops
  auto t = _population.agent(to);
  for (auto c : _neighbors[from])
    if (c == t) return;
  _neighbors[from].push_back(t);
  _neighbors[to].push_back(_population.agent(from));
}

void ConfigurationModel::grow(const PAgent &agent)
{
  stop("not implemented yet");
}

SEXP newConfigurationModel(SEXP population, SEXP rng)
{
  XP<Population> p(population);
  Function f(rng);
  return XP<ConfigurationModel>(std::make_shared<ConfigurationModel>(**p, f));
}
