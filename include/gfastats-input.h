//
//  gfastats-input.h
//  gfastats
//
//  Created by Giulio Formenti on 1/16/22.
//

#ifndef GFASTATS_INPUT_H
#define GFASTATS_INPUT_H

class Input {
    
    UserInput userInput;
    
    //intermediates
    std::string h;
    char* c;
    
public:
    
    void load(UserInput userInput);
    
    void read(InSequences& inSequence);
    
    Sequence* includeExcludeSeq(std::string seqHeader, std::string seqComment, std::string* inSequence, BedCoordinates bedIncludeList, BedCoordinates bedExcludeList, std::string* inSequenceQuality = NULL);

    Sequence* includeExcludeSeg(InSequences* inSequences, std::string* seqHeader, std::string* seqComment, std::string* inSequence, BedCoordinates bedIncludeList, BedCoordinates bedExcludeList, std::string* inSequenceQuality = NULL, std::vector<Tag>* inTags = NULL);

friend class Input;
    
};

#endif /* GFASTATS_INPUT_H */
