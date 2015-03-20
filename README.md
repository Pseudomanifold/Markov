This document is an almost verbatim copy of [one of my blog posts](http://bastian.rieck.ru/blog/posts/2014/markov_chains).

I have always been fascinated by [Markov chains](https://en.wikipedia.org/wiki/Markov_chain) for text
generation. Despite their simple structure, a sufficiently large body of text will result in
strikingly interesting products. Naturally, I had to provide my own implementation for simple text
generation.

# Algorithm

My algorithm first tokenizes each input file. I consider each punctuation mark to be a token as well
because the results sound slightly more natural that way. This treatment requires me to check for
punctuation marks when writing the text to STDOUT because punctuation marks should of course not
introduce additional whitespace.

Having tokenized the file, I extract prefixes of a certain length (say 2) and map them to the
subsequent. This yields a function from the set of prefixes to the set of potential candidate words.
To start the Markov chain generation, I choose a prefix at random and choose a new candidate using a
uniform distribution. Since I do not check for duplicates in the list of candidate words, I do not
need to take relative frequencies into account. The old prefix is updated to include the new
candidate. This yields a new prefix from which I may choose a new candidate, and so on.

I opted for the lazy implementation and simple repeat this step until a predefined number of
iterations has been reached.

# Examples

This is an example of a new Christmas song. Some of the lines appear to be taken verbatim from a
song because there is too little text for the algorithm to choose from. Still, the result is quite
nice:

> Mary, and the bitter weather. Sire, the baby awakes,
> But the fire is so hard to sleep tonight
> They know that Santa's on his sleigh
> And ev'ry mother's child is gonna spy
> To see if reindeer really know how to fly
> And so, I'm Telling you why: Santa Claus comes tonight.
> Here were are as follows,
> and on my back I fell;
> A gent was riding by, in a pear tree.

Using the King James bible (and hence a larger text corpus), the result sounds rather ominous:

> Restrained they the things that are desolate in the eyes of my people, then thou shalt take all
> the inhabitants of the land of Judah, and fall; and they opened their mouths, that they might know
> thee, O LORD, I shall eat of the giant.
> 21:19 And it came to pass.
> 24:13 And I went to Pilate, and they let her own, and the vail that is written, He that hath not
> sent to the wilderness of Paran.

# Usage and code

    $ mkdir build
    $ cd build
    $ cmake ../
    $ make
    $ markov --prefix 2 --iterations 10 ../Christmas.txt

Anything that does not belong to either one of the *optional* `--prefix` or `--iterations`
parameters is considered a filename. The program currently combines all files into a large database.
This is subject to change.

This program is released under the MIT licence.
