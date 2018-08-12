const fs = require('fs')
const _ = require('lodash/fp')

const wordList = fs.readFileSync('./wordlist.txt', 'utf8')
const words = _.map(_.truncate({length: 4, omission: ''}), wordList.split('\n').slice(0, -1))

const lookup = []

function createLookup () {
  for (let i = 0; i < words.length; i++) {
    const word = words[i]
    const wordLookup = {}

    for (let j = 0; j < word.length; j++) {
      const letter = word[j]
      wordLookup[letter] = (wordLookup[letter] || 0) + 1
    }

    lookup[i] = wordLookup
  }
}

function computeRing (ring) {
  let maxOccurances = 0
  let biggest

  for (let i = 0; i < words.length; i++) {
    const wordLookup = lookup[i]
    let occurances = 0

    for (let j = 0; j < ring.length; j++) {
      const ringLetter = ring[j]
      occurances += (wordLookup[ringLetter] || 0)
    }

    if (occurances > maxOccurances) {
      maxOccurances = occurances
      biggest = i
    }
    maxOccurances = _.max([occurances, maxOccurances])
  }

  console.log('rings: %d, word: %s', maxOccurances, words[biggest])
  return maxOccurances
}

function computeConfig (rings) {
  let sum = 0

  for (let i = 0; i < 5; i++) {
    sum += computeRing(rings[i])
  }

  console.log(12 * sum)
}

createLookup()
// const srings = [
//   'bcdfln',
//   'kmpqvz',
//   'aeiouy',
//   'ghrstw',
//   'jx'
// ]

// const _rings = `hnvxyz
// bcdrtw
// aeikou
// fglmps
// jq`

const _rings = `hnvxyz
bcdrtw
aeikou
fglmps
jq`

const ssrings = _.map(_.split(''), _.split('\n', _rings))

console.log(ssrings)
computeConfig(ssrings)
