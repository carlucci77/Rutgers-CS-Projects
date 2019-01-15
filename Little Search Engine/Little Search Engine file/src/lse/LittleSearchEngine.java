package lse;
import java.io.*;
import java.util.*;
/**
 * This class builds an index of keywords. Each keyword maps to a set of pages in
 * which it occurs, with frequency of occurrence in each page.
 *
 */
public class LittleSearchEngine {
	/**
	 * This is a hash table of all keywords. The key is the actual keyword, and the associated value is
	 * an array list of all occurrences of the keyword in documents. The array list is maintained in 
	 * DESCENDING order of frequencies.
	 */
	HashMap<String,ArrayList<Occurrence>> keywordsIndex;
	/**
	 * The hash set of all noise words.
	 */
	HashSet<String> noiseWords;
	/**
	 * Creates the keyWordsIndex and noiseWords hash tables.
	 */
	public LittleSearchEngine() {
		keywordsIndex = new HashMap<String,ArrayList<Occurrence>>(1000,2.0f);
		noiseWords = new HashSet<String>(100,2.0f);
	}
	/**
	 * Scans a document, and loads all keywords found into a hash table of keyword occurrences
	 * in the document. Uses the getKeyWord method to separate keywords from other words.
	 * 
	 * @param docFile Name of the document file to be scanned and loaded
	 * @return Hash table of keywords in the given document, each associated with an Occurrence object
	 * @throws FileNotFoundException If the document file is not found on disk
	 */
	public HashMap<String,Occurrence> loadKeywordsFromDocument(String docFile) 
	throws FileNotFoundException {
		String word = "";
		File document = new File(docFile);
		if(!document.exists()) {
			throw new FileNotFoundException();
		}
		Scanner scan = new Scanner(document);
		HashMap<String, Occurrence> keyWords = new HashMap<String, Occurrence>();
		while(scan.hasNext()) {
			word = getKeyword(scan.next());
			if(word != null) {
				if(keyWords.containsKey(word)) {
					keyWords.get(word).frequency ++;
				}else {
					Occurrence occur = new Occurrence(docFile, 1);
					keyWords.put(word, occur);
				}
			}
		}
		scan.close();
		return keyWords;
	}
	/**
	 * Merges the keywords for a single document into the master keywordsIndex
	 * hash table. For each keyword, its Occurrence in the current document
	 * must be inserted in the correct place (according to descending order of
	 * frequency) in the same keyword's Occurrence list in the master hash table. 
	 * This is done by calling the insertLastOccurrence method.
	 * 
	 * @param kws Keywords hash table for a document
	 */
	public void mergeKeywords(HashMap<String,Occurrence> kws) {
		Iterator<String> traverse = kws.keySet().iterator();
		String keyword = "";
		while(traverse.hasNext()) {
			keyword = traverse.next();
			if(keywordsIndex.containsKey(keyword)) {
				keywordsIndex.get(keyword).add(kws.get(keyword));
				insertLastOccurrence(keywordsIndex.get(keyword));
			}else {
				ArrayList<Occurrence> occs = new ArrayList<Occurrence>();
				occs.add(kws.get(keyword));
				keywordsIndex.put(keyword, occs);
			}
		}
	}
	/**
	 * Given a word, returns it as a keyword if it passes the keyword test,
	 * otherwise returns null. A keyword is any word that, after being stripped of any
	 * trailing punctuation, consists only of alphabetic letters, and is not
	 * a noise word. All words are treated in a case-INsensitive manner.
	 * 
	 * Punctuation characters are the following: '.', ',', '?', ':', ';' and '!'
	 * 
	 * @param word Candidate word
	 * @return Keyword (word without trailing punctuation, LOWER CASE)
	 */
	public String getKeyword(String word) {
		int index = 0;
		String extra = "";
		String word1 = word;
		if(word ==  null) {
			return null;
		}
		word1 = word.toLowerCase();
		for(int k = word1.length() - 1; k >= 0; k--) {
			if(word1.charAt(k) >= 'a' && word1.charAt(k) <= 'z') {
				index = k;
				break;
			}
		}
		extra = word1.substring(index + 1);
		word1 = word1.substring(0, index + 1);
		for(int j = 0; j < extra.length(); j++) {
			if(extra.charAt(j) != '.' && extra.charAt(j) != '!' && extra.charAt(j) != ',' && extra.charAt(j) != '?' 
					&& extra.charAt(j) != ';' && extra.charAt(j) != ':') {
				return null;
			}
		}
		for(int i = 0; i < word1.length(); i++) {
			if(!(word1.charAt(i) >= 'a' && word1.charAt(i) <= 'z')) {
				return null;
			}
		}
		if(noiseWords.contains(word1)) {
			return null;
		}
		return word1;
	}
	/**
	 * Inserts the last occurrence in the parameter list in the correct position in the
	 * list, based on ordering occurrences on descending frequencies. The elements
	 * 0..n-2 in the list are already in the correct order. Insertion is done by
	 * first finding the correct spot using binary search, then inserting at that spot.
	 * 
	 * @param occs List of Occurrences
	 * @return Sequence of mid point indexes in the input list checked by the binary search process,
	 *         null if the size of the input list is 1. This returned array list is only used to test
	 *         your code - it is not used elsewhere in the program.
	 */
	public ArrayList<Integer> insertLastOccurrence(ArrayList<Occurrence> occs) {
		if(occs.size() <= 1) {
			return null;
		}
		Occurrence insert = occs.get(occs.size() - 1);
		int index = 0;
		int start = 0;
		int end = occs.size() - 2;
		ArrayList<Integer> midpoints = new ArrayList<Integer>();
		while (start <= end) {
			int mid = (start + end) / 2;
			midpoints.add(mid);
			if (insert == occs.get(mid)) {
				index = mid + 1;
				break;
			}
			if (insert.frequency > occs.get(mid).frequency) {
				end = mid - 1;
			} else {
				start = mid + 1;
			}
			index = mid;
		}
		occs.remove(occs.size() - 1);
		if(insert.frequency <= occs.get(index).frequency) {
			occs.add(index + 1, insert);
		}else {
			occs.add(index, insert);
		}
		return midpoints;
	}
	/**
	 * This method indexes all keywords found in all the input documents. When this
	 * method is done, the keywordsIndex hash table will be filled with all keywords,
	 * each of which is associated with an array list of Occurrence objects, arranged
	 * in decreasing frequencies of occurrence.
	 * 
	 * @param docsFile Name of file that has a list of all the document file names, one name per line
	 * @param noiseWordsFile Name of file that has a list of noise words, one noise word per line
	 * @throws FileNotFoundException If there is a problem locating any of the input files on disk
	 */
	public void makeIndex(String docsFile, String noiseWordsFile) 
	throws FileNotFoundException {
		// load noise words to hash table
		Scanner sc = new Scanner(new File(noiseWordsFile));
		while (sc.hasNext()) {
			String word = sc.next();
			noiseWords.add(word);
		}
		// index all keywords
		sc = new Scanner(new File(docsFile));
		while (sc.hasNext()) {
			String docFile = sc.next();
			HashMap<String,Occurrence> kws = loadKeywordsFromDocument(docFile);
			mergeKeywords(kws);
		}
		sc.close();	
	}
	/**
	 * Search result for "kw1 or kw2". A document is in the result set if kw1 or kw2 occurs in that
	 * document. Result set is arranged in descending order of document frequencies. (Note that a
	 * matching document will only appear once in the result.) Ties in frequency values are broken
	 * in favor of the first keyword. (That is, if kw1 is in doc1 with frequency f1, and kw2 is in doc2
	 * also with the same frequency f1, then doc1 will take precedence over doc2 in the result. 
	 * The result set is limited to 5 entries. If there are no matches at all, result is null.
	 * 
	 * @param kw1 First keyword
	 * @param kw1 Second keyword
	 * @return List of documents in which either kw1 or kw2 occurs, arranged in descending order of
	 *         frequencies. The result size is limited to 5 documents. If there are no matches, returns null.
	 */
	public ArrayList<String> top5search(String kw1, String kw2) {
		ArrayList<String> result = new ArrayList<String>();
		ArrayList<Occurrence> kw1List = new ArrayList<Occurrence>();
		ArrayList<Occurrence> kw2List = new ArrayList<Occurrence>();
		String kw1Doc = "";
		String kw2Doc = "";
		if(!keywordsIndex.containsKey(kw1) && !keywordsIndex.containsKey(kw2)) {
			return null;
		}else if(keywordsIndex.containsKey(kw1) && !keywordsIndex.containsKey(kw2)) {
			kw1List = keywordsIndex.get(kw1);
			for(int k = 0; k < kw1List.size(); k++) {
				if(result.size() < 5) {
					kw1Doc = kw1List.get(k).document;
					if(result.contains(kw1Doc)){
						continue;
					}else {
						result.add(kw1Doc);
					}
				}else {
					break;
				}
			}
		}else if(!keywordsIndex.containsKey(kw1) && keywordsIndex.containsKey(kw2)) {
			kw2List = keywordsIndex.get(kw2);
			for(int k = 0; k < kw2List.size(); k++) {
				if(result.size() < 5) {
					kw2Doc = kw2List.get(k).document;
					if(result.contains(kw2Doc)) {
						continue;
					}else {
						result.add(kw2Doc);
					}
				}else {
					break;
				}
			}
		}else {
			kw1List = keywordsIndex.get(kw1);
			kw2List = keywordsIndex.get(kw2);
			int Index1 = 0;
			int Index2 = 0;
			while(Index1 < kw1List.size() && Index2 < kw2List.size()) {
				if(result.size() < 5) {
					if(kw1List.get(Index1).frequency > kw2List.get(Index2).frequency) {
						if(!result.contains(kw1List.get(Index1).document)) {
							result.add(kw1List.get(Index1).document);
						}
						Index1 ++;
					}else if(kw1List.get(Index1).frequency < kw2List.get(Index2).frequency) {
						if(!result.contains(kw2List.get(Index2).document)) {
							result.add(kw2List.get(Index2).document);
						}
						Index2++;
					}else {
						if(!result.contains(kw1List.get(Index1).document)) {
							result.add(kw1List.get(Index1).document);
						}
						Index1++;
					}
				}else {
					break;
				}
			}
			while(Index1 < kw1List.size()) {
				if(result.size() < 5) {
					if(!result.contains(kw1List.get(Index1).document)) {
						result.add(kw1List.get(Index1).document);
					}
					Index1++;
				}else {
					break;
				}
			}
			while(Index2 < kw2List.size()) {
				if(result.size() < 5) {
					if(!result.contains(kw2List.get(Index2).document)) {
						result.add(kw2List.get(Index2).document);
					}
					Index2++;
				}else {
					break;
				}
			}
		}
		return result;
	}
}