#pragma once

#include "Agent.h"
#include "Contact.h"
#include <vector>
#include <string>
#include <list>
#include <utility>
#include <Rcpp.h>

/**
 * The class that represnts a population
 * 
 * A population is a collection of agents. There are two important tasks
 * for a population:
 *   1. to manage the agents in it
 *   2. to define the contact patterns of the agents
 *   
 * The contact patterns are defined by objects of the Contact class that
 * are associated with the population. A population may have multiple 
 * Contact objects, for example, one for random mixing, one for close
 * contacts represented by a contact netowrk, and another for social
 * network.
 */
class Population : public Agent {
public:
  /**
   * Constructor 
   * 
   * @param n the initial population size
   * 
   * @details The population will be created with "n" individuals in it.
   * These individuals have an empty state upon created. Note that 
   * individuals can be added later by the "add" method, the initial
   * population size is for convenience, not required.
   */
  Population(size_t n = 0);

  /**
   * Add an agent to the populaton
   * 
   * @param agent A shared_ptr<Agent> pointing to the agent to be added
   * 
   * @details The agent is scheduled in the population. If the population 
   * is already added to a simulation, the agent will report its state
   * to the simulation.
   */
  void add(PAgent agent);
  
  /**
   * Add a contact pattern
   * 
   * @param contact A shared_pre<Contact> pointing to a Contact object.
   * 
   * @details If the contact has already been added, this called does 
   * nothing.
   */
  void add(PContact contact);
  
  /**
   * the population size
   * 
   * It is the number of agents in the population.
   */
  size_t size() const { return _agents.size(); }
  /**
   * return a specific agent by index
   * 
   * @param i the index of the agent (starting from 0)
   * 
   * @return a shared_ptr<Agent> pointing to the agent requested.
   */
  PAgent agent(size_t i) const { return _agents[i]; }

  /**
   * Initialize the state of agents in the population using an 
   * initializer function
   * 
   * @param init an R initializer function. Pleae see details section.
   * 
   * @details The R initializer function should take a single argument
   * specifying the index of the agent to be initialized (starting from 1
   * following R tradition). It should return the initial state of the 
   * agent.
   */
  void initialize(Rcpp::Function init);
  
  /**
   * The classes of an Agent object
   */
  static Rcpp::CharacterVector classes;

  /**
   * reports its state to the simulation.
   * 
   * @details This function should not be called by the user. It is
   * automatically called by the simulation when it needs to collect
   * the state information from the agent.
   */
  virtual void report();

protected:
  /**
   * A vector holding all agents in the population
   */
  std::vector<PAgent> _agents;
  /**
   * A list of shared_ptr<Contact> pointing to the contacts
   */
  std::list<PContact> _contacts;
};

typedef std::shared_ptr<Population> PPopulation;

extern "C"{
  /**
   * Creates a new population with n agents
   * 
   * @param n the initial number of agents in the population. A 
   * non-negative integer. Can be NULL, which means n=0.
   * 
   * @return an external pointer pointing to a Populaation object
   * 
   * @details The agents have empty states. Because agents can be
   * added later to the population using the AddAgent function, the
   * initial population is a convenience instead of a requirement.
   */
  SEXP newPopulation(SEXP n);
  
  /**
   * Add an agent to the population
   * 
   * @param population an external pointer pointing to the population
   * for the agent to be added to.
   * 
   * @param agent an external pointer pointing to the agent to be added.
   * 
   * @return the same as population.
   * 
   * @details The agent is scheduled in the population. If the population 
   * is already added to a simulation, the agent will report its state
   * to the simulation.
   */
  SEXP addAgent(SEXP population, SEXP agent);

  /**
   * Get the population size
   * 
   * @param population an external pointer pointing to the population
   * for the agent to be added to.
   * 
   * @return the number of individuals in the population as na integer
   */
  SEXP getSize(SEXP population);

  /**
   * get the agent at a specific index in population 
   * 
   * @param population an external pointer pointing to the population
   * for the agent to be added to.
   * 
   * @param i the index of the agent to be retrieved. A non-negative 
   * integer
   * 
   * @return an external pointer pointing to the agent
   */
  SEXP getAgent(SEXP population, SEXP i);
  
  /**
   * Adds a Contact object to a population
   * 
   * @param population an external pointer pointing to the population
   * for the agent to be added to.
   * 
   * @param contact an external pointer pointing to the Contact
   * object to be added.
   * 
   * @return the same as population
   */
  SEXP addContact(SEXP population, SEXP contact);
}
