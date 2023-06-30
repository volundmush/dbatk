#pragma once
#include "dbatk/base.h"
#include "dbatk/aspects/sex.h"

/*
 * Virtually all of this code is adapted from Evennia's
 * https://github.com/evennia/evennia/tree/main/evennia/utils/verb_conjugation
 *
 * Credit where credit is due.
 * */

namespace dbat::grammar {

    enum VerbTense : std::size_t {
        Infinitive = 0,
        FirstSingularPresent = 1,
        SecondSingularPresent = 2,
        ThirdSingularPresent = 3,
        PresentPlural = 4,
        PresentParticiple = 5,
        FirstSingularPast = 6,
        SecondSingularPast = 7,
        ThirdSingularPast = 8,
        PastPlural = 9,
        Past = 10,
        PastParticiple = 11
    };

    enum VerbPerson : std::size_t {
        FirstPerson = 0,
        SecondPerson = 1,
        ThirdPerson = 2,
        Plural = 3,
        PresPlural = 4
    };

    extern std::unordered_map<std::string, std::vector<std::string>> tenses;
    extern std::unordered_map<std::string, std::string> lemmas;

    std::string getInfinitive(const std::string &verb);
    std::string getConjugate(const std::string& verb, VerbTense tense=Infinitive, bool negate=false);
    std::string getPresent(const std::string& verb, VerbPerson person=FirstPerson, bool negate=false);
    std::string getPresentParticiple(const std::string& verb);
    std::string getPast(const std::string& verb, VerbPerson person=FirstPerson, bool negate=false);
    std::string getPastParticiple(const std::string& verb);

    std::optional<VerbTense> getTense(const std::string& verb);
    std::pair<std::string, std::string> getConjugateCandidates(const std::string& verb);

    void registerWords();

    // Pronouns section...

    enum PronounType : std::size_t {
        Subjective = 0,
        Objective = 1,
        PossessiveAdjective = 2,
        PossessivePronoun = 3,
        Reflexive = 4
    };

    /*

        ====================  =======  ========  ==========  ==========  ===========
        viewpoint/pronouns    Subject  Object    Possessive  Possessive  Reflexive
                              Pronoun  Pronoun   Adjective   Pronoun     Pronoun
        ====================  =======  ========  ==========  ==========  ===========
        1st person              I        me        my         mine       myself
        1st person plural       we       us        our        ours       ourselves
        2nd person              you      you       your       yours      yourself
        2nd person plural       you      you       your       yours      yourselves

        3rd person male         he       him       his        his        himself
        3rd person female       she      her       her        hers       herself
        3rd person neutral      it       it        its        its        itself
        3rd person plural       they     them      their      theirs     themselves
        ====================  =======  ========  ==========  ==========  ===========

     */

    std::string getPronoun(sex::SexId sex, VerbPerson person, PronounType pType);

    std::string an(const std::string &word);

    std::string capitalize(const std::string &word);

}