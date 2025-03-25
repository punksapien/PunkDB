# citation

this implementation is based on the research paper:

## the case for learned index structures

**authors:** tim kraska, alex beutel, ed h. chi, jeffrey dean, neoklis polyzotis

**conference:** sigmod 2018 (international conference on management of data)

**location:** houston, tx, usa

**date:** june 10-15, 2018

**pages:** 489-504

**doi:** https://doi.org/10.1145/3183713.3196909

**arxiv:** https://arxiv.org/abs/1712.01208

---

### abstract

the paper proposes that indexes are models: a b-tree can be seen as a model to map a key to the position of a record within a sorted array. it shows that by using neural nets, learned indexes can outperform cache-optimized b-trees by up to 70% in speed while saving an order-of-magnitude in memory.

---

### citation formats

**acm format:**
```
tim kraska, alex beutel, ed h. chi, jeffrey dean, and neoklis polyzotis. 2018.
the case for learned index structures. in proceedings of the 2018 international
conference on management of data (sigmod '18), june 10-15, 2018, houston, tx, usa.
acm, 489-504. https://doi.org/10.1145/3183713.3196909
```

**bibtex:**
```bibtex
@inproceedings{kraska2018case,
  title={The case for learned index structures},
  author={Kraska, Tim and Beutel, Alex and Chi, Ed H and Dean, Jeffrey and Polyzotis, Neoklis},
  booktitle={Proceedings of the 2018 International Conference on Management of Data},
  pages={489--504},
  year={2018},
  organization={ACM},
  doi={10.1145/3183713.3196909}
}
```

**apa format:**
```
kraska, t., beutel, a., chi, e. h., dean, j., & polyzotis, n. (2018, june).
the case for learned index structures. in proceedings of the 2018 international
conference on management of data (pp. 489-504). acm.
```

---

### key contributions

1. **indexes as models** - reframes traditional index structures as ml models
2. **learned indexes** - uses neural networks and linear regression to predict record positions
3. **performance gains** - up to 70% faster than b-trees with 10x less memory
4. **learned bloom filters** - 30% smaller than traditional bloom filters

this implementation demonstrates the core concept using simple linear regression for key position prediction.
