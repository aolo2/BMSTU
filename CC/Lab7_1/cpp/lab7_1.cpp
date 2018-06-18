#include "tree.hpp"

#include <sstream>
#include <fstream>
#include <deque>
#include <stack>

std::vector<int32_t> TRANSITION[20][13] =
/*                      EOP                        K_NONTERMINAL                   K_TERMINAL                 K_AXIOM                   K_EPSILON                  OR                         QUOAD_EQUALS                  QUOTE                      SEMICOLON                  COMMA                      NT_NAME                    LC_WORD                    SYMBOL                    */
{
/* ERROR */           { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* GRAMMAR */         { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* N_LIST */          { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* N_LIST_ITEMS */    { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* N_LIST_ITEMS_2 */  { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* NON_TERMINAL */    { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* T_LIST */          { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* T_LIST_ITEMS */    { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* T_LIST_ITEMS_2 */  { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* TERMINAL */        { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* RULES */           { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* RULES_2 */         { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* RULE */            { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* RIGHT_PART */      { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* ALTERNATIVES */    { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* ALTERNATIVE */     { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* TN */              { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* TN_LIST */         { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* TN_LIST_2 */       { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } },
/* AXIOM */           { { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },      { (int32_t) (N::ERROR) },   { (int32_t) (N::ERROR) }, { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },     { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) },  { (int32_t) (N::ERROR) } }
};

static void init()
{
  TRANSITION[N::GRAMMAR][T::K_NONTERMINAL] = { (int32_t) (N::N_LIST), (int32_t) (N::T_LIST), (int32_t) (N::RULES), (int32_t) (N::AXIOM) };

  TRANSITION[N::N_LIST][T::K_NONTERMINAL] = { -1 * (int32_t) (T::K_NONTERMINAL), (int32_t) (N::N_LIST_ITEMS), -1 * (int32_t) (T::SEMICOLON) };

  TRANSITION[N::N_LIST_ITEMS][T::NT_NAME] = { -1 * (int32_t) (T::NT_NAME), (int32_t) (N::N_LIST_ITEMS_2) };

  TRANSITION[N::N_LIST_ITEMS_2][T::COMMA] = { -1 * (int32_t) (T::COMMA), -1 * (int32_t) (T::NT_NAME), (int32_t) (N::N_LIST_ITEMS_2) };
  TRANSITION[N::N_LIST_ITEMS_2][T::SEMICOLON] = { };

  TRANSITION[N::T_LIST][T::K_TERMINAL] = { -1 * (int32_t) (T::K_TERMINAL), (int32_t) (N::T_LIST_ITEMS), -1 * (int32_t) (T::SEMICOLON) };

  TRANSITION[N::T_LIST_ITEMS][T::QUOTE] = { (int32_t) (N::TERMINAL), (int32_t) (N::T_LIST_ITEMS_2) };
  TRANSITION[N::T_LIST_ITEMS][T::LC_WORD] = { (int32_t) (N::TERMINAL), (int32_t) (N::T_LIST_ITEMS_2) };

  TRANSITION[N::T_LIST_ITEMS_2][T::COMMA] = { -1 * (int32_t) (T::COMMA), (int32_t) (N::TERMINAL), (int32_t) (N::T_LIST_ITEMS_2) };
  TRANSITION[N::T_LIST_ITEMS_2][T::SEMICOLON] = { };

  TRANSITION[N::TERMINAL][T::QUOTE] = { -1 * (int32_t) (T::QUOTE), -1 * (int32_t) (T::SYMBOL), -1 * (int32_t) (T::QUOTE) };
  TRANSITION[N::TERMINAL][T::LC_WORD] = { -1 * (int32_t) (T::LC_WORD) };

  TRANSITION[N::RULES][T::NT_NAME] = { (int32_t) (N::RULE), (int32_t) (N::RULES_2) };

  TRANSITION[N::RULES_2][T::NT_NAME] = { (int32_t) (N::RULE), (int32_t) (N::RULES_2) };
  TRANSITION[N::RULES_2][T::SEMICOLON] = {};

  TRANSITION[N::RULE][T::NT_NAME] = { -1 * (int32_t) (T::NT_NAME), -1 * (int32_t) (T::QUAD_EQUALS), (int32_t) (N::RIGHT_PART), -1 * (int32_t) (T::SEMICOLON) };

  TRANSITION[N::RIGHT_PART][T::QUOTE] = { (int32_t) (N::ALTERNATIVE), (int32_t) (N::ALTERNATIVES) };
  TRANSITION[N::RIGHT_PART][T::NT_NAME] = { (int32_t) (N::ALTERNATIVE), (int32_t) (N::ALTERNATIVES) };
  TRANSITION[N::RIGHT_PART][T::LC_WORD] = { (int32_t) (N::ALTERNATIVE), (int32_t) (N::ALTERNATIVES) };
  TRANSITION[N::RIGHT_PART][T::K_EPSILON] = { -1 * (int32_t) (T::K_EPSILON) };

  TRANSITION[N::ALTERNATIVES][T::OR] = { -1 * (int32_t) (T::OR), (int32_t) (N::ALTERNATIVE), (int32_t) (N::ALTERNATIVES) };
  TRANSITION[N::ALTERNATIVES][T::SEMICOLON] = {};

  TRANSITION[N::ALTERNATIVE][T::QUOTE] = { (int32_t) (N::TN), (int32_t) (N::TN_LIST) };
  TRANSITION[N::ALTERNATIVE][T::NT_NAME] = { (int32_t) (N::TN), (int32_t) (N::TN_LIST) };
  TRANSITION[N::ALTERNATIVE][T::LC_WORD] = { (int32_t) (N::TN), (int32_t) (N::TN_LIST) };
  TRANSITION[N::ALTERNATIVE][T::K_EPSILON] = { (int32_t) (N::TN), (int32_t) (N::TN_LIST) };

  TRANSITION[N::TN][T::QUOTE] = { (int32_t) (N::TERMINAL) };
  TRANSITION[N::TN][T::LC_WORD] = { (int32_t) (N::TERMINAL) };
  TRANSITION[N::TN][T::NT_NAME] = { -1 * (int32_t) (T::NT_NAME) };

  TRANSITION[N::TN_LIST][T::QUOTE] = { (int32_t) (N::TN), (int32_t) (N::TN_LIST_2) };
  TRANSITION[N::TN_LIST][T::LC_WORD] = { (int32_t) (N::TN), (int32_t) (N::TN_LIST_2) };
  TRANSITION[N::TN_LIST][T::NT_NAME] = { (int32_t) (N::TN), (int32_t) (N::TN_LIST_2) };
  TRANSITION[N::TN_LIST][T::K_EPSILON] = { (int32_t) (N::TN), (int32_t) (N::TN_LIST_2) };

  TRANSITION[N::TN_LIST_2][T::QUOTE] = { (int32_t) (N::TN), (int32_t) (N::TN_LIST_2) };
  TRANSITION[N::TN_LIST_2][T::LC_WORD] = { (int32_t) (N::TN), (int32_t) (N::TN_LIST_2) };
  TRANSITION[N::TN_LIST_2][T::NT_NAME] = { (int32_t) (N::TN), (int32_t) (N::TN_LIST_2) };
  TRANSITION[N::TN_LIST_2][T::K_EPSILON] = { (int32_t) (N::TN), (int32_t) (N::TN_LIST_2) };
  TRANSITION[N::TN_LIST_2][T::SEMICOLON] = {};

  TRANSITION[N::AXIOM][T::K_AXIOM] = { -1 * (int32_t) (T::K_AXIOM), -1 * (int32_t) (T::NT_NAME), -1 * (int32_t) (T::SEMICOLON) };
}

static tree
parse(std::deque<int32_t> &tokens)
{
  tree result;
  tree *active_node = &result;
  std::stack<parse_pair> magazine;

  magazine.push({ -1 * (int32_t) (T::EOP), nullptr }); // NOTE(aolo2): nullptr parent will not be dereferenced
  magazine.push({ (int32_t) (N::GRAMMAR), &result });

  int32_t a = tokens.front();
  int32_t X = magazine.top().sym;

  while (X != 0) // NOTE(aolo2): (symbol) (T::EOP) = 0
  {
    if (is_terminal(X))
    {
      if (X == a)
      {
        add_child(active_node, tree(true, X, {})); // NOTE(aolo2): return value is ingored as leaf nodes have no children
        magazine.pop();
        tokens.pop_front();
      }
      else
      {
        assert(std::string("UNEXPECTED TERMINAL") == std::string("OK"));
        // TODO(aolo2): error
      }
    }
    else
    {
      assert(X >= 0 && a <= 0);
      auto rule = TRANSITION[X][(-1) * a]; // NOTE(aolo2): X \in [1, ...]

      // TODO(aolo2): check oob instead of if (true)-ing
      if (true)
      {
        magazine.pop();
        for (uint32_t i = 0; i < rule.size(); ++i)
        {
          auto sym = rule[rule.size() - 1 - i];
          tree *child = add_child(active_node);
          magazine.push({ sym, child });
        }
      }
      else
      {
        // TODO(aolo2): error
        assert(std::string("UNEXPECTED RULE") == std::string("OK"));
      }
    }

    a = tokens.front();
    X = magazine.top().sym;
    active_node = magazine.top().node;
  }

  return(result);
}

int
main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: ./lab7 INPUT" << std::endl;
    return(1);
  }

  std::ifstream file(argv[1]);
  std::stringstream sstream;
  std::string text;
  std::vector<std::string> errors;

  sstream << file.rdbuf();
  text = sstream.str();

  init();

  // TODO(aolo2): errors
  auto tokens = lex(text, errors);

  std::deque<int32_t> symbols;
  for (const auto &tok : tokens)
  {
    symbols.push_front( -1 * ((int32_t) tok.tag));
  }

  auto tree = parse(symbols);
  print_tree(tree);

#if 0
  for (const auto &tok : tokens)
    std::cout << token_name((T) tok.tag) << " " << token_payload(tok, text) << std::endl;
#endif

  return(0);
}