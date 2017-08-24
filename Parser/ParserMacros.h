#ifndef ParserMacros_H
#define ParserMacros_H

#define PARSER_DEFINE(Name) public: const size_t type##Name = typeId++; private: bool parse##Name(size_t parent)
#define PARSER_DEFINE_RANGE(Name, from, to) PARSER_DEFINE(Name) { return stringSequencer.isInRange(from, to); }
#define PARSER_DEFINE_ONE_OF(Name, set) PARSER_DEFINE(Name) { return stringSequencer.isOneOf(set); }
#define PARSER_DEFINE_SEQUENCE(Name, sequence) PARSER_DEFINE(Name) { return stringSequencer.isSequence(sequence); }
#define PARSER_PARSE(Name) (parseElement([&](size_t parent) -> bool { return parse##Name(parent); }, type##Name, parent, false))
#define PARSER_PARSE_SILENT(Name) (parseElement([&](size_t parent) -> bool { return parse##Name(parent); }, type##Name, parent, true))
#define PARSER_PARSE_STOP() (stringSequencer.isEndOfFile())
#define PARSER_ADVANCE() (stringSequencer.advance())
#define PARSER_INIT(ParserName, EntryPoint) public: \
    virtual bool parse() { size_t parent = Parser::nullParent; return PARSER_PARSE(EntryPoint); } \
    ParserName(ObjectTree &objectTree, StringSequencer &stringSequencer) : Parser(objectTree, stringSequencer)

#endif
