const fs = require('fs')
const _ = require('lodash/fp')

const alphabet = _.split('', 'abcdefghijklmnopqrstuvwxyz')
const wordList = fs.readFileSync('./wordlist.txt', 'utf8')
const words = _.map(_.truncate({length: 4, omission: ''}), wordList.split('\n').slice(0, -1))

const lookup = []

// For each ring i in configuration: find word from list that uses the most of the rings, call this number x(i)
// Take 12 * sum(x(i)) -- that's how many total rings we need.

function buildConfig () {
  let ab = _.clone(alphabet)
  const config = []
  for (let i = 0; i < 5; i++) {
    const ringLetters = []
    const letterCount = i === 4 ? 2 : 6
    for (let j = 0; j < letterCount; j++) {
      const n = Math.floor(Math.random() * ab.length)
      ringLetters[j] = ab[n]
      ab[n] = null
      ab = _.compact(ab)
    }

    ringLetters.sort()
    config[i] = ringLetters
  }

  return config
}

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

  for (let i = 0; i < words.length; i++) {
    const wordLookup = lookup[i]
    let occurances = 0

    for (let j = 0; j < ring.length; j++) {
      const ringLetter = ring[j]
      occurances += (wordLookup[ringLetter] || 0)
    }

    maxOccurances = _.max([occurances, maxOccurances])
  }

  return maxOccurances
}

function computeConfig (rings) {
  let sum = 0

  for (let i = 0; i < 5; i++) {
    sum += computeRing(rings[i])
  }

  return 12 * sum
}

createLookup()

let minResult = 1000
let i = 0
let t0 = Date.now()

while (true) {
  let srings = buildConfig()
  let result = computeConfig(srings)

  if (result < minResult) {
    console.log(result)
    console.log(srings)
  }

  i++

  if (Date.now() - t0 > 10000) {
    t0 = Date.now()
    console.log(i)
  }

  minResult = _.min([minResult, result])
}
