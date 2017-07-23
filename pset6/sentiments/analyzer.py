import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        # Declaration
        self.positive = set()
        self.negative = set()
        
        # Opens positives and loads it into library
        file = open(positives, "r")
        for line in file:
            if not line.startswith(";"):
                self.positive.add(line.rstrip())
        file.close()
        
        # Opens negatives and loads it into library
        file = open(negatives, "r")
        for line in file:
            if not line.startswith(";"):
                self.negative.add(line.rstrip())
        file.close()
    
        return

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        # parse tweets to make it processible and tokenise string
        thnzr = nltk.tokenize.casual.TweetTokenizer(strip_handles=True, reduce_len=True)
        
        #init score
        score = 0
        
        # iterate through tokens
        for object in thnzr.tokenize(text):
            if object in self.positive:
                score += 1
            if object in self.negative:
                score -= 1
        
        #return
        return score

if __name__ == "__main__":
    main()