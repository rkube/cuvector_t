A testbed for vector classes

especially test the following ideas

Implement iterators that have dynamic boundaries,i.e.

vec.size = n + pad

vec.set_transformed(0, false)
for(it : vec)
    std::cout << *it

prints vec[0], ..., vec[n - 1]

vec.set_transformed(0, true)
for(it : vec)
    std::cout << *it

prints vec[0], ..., vec[n - 1], ... vec[n + pad - 1]




Implement ghost points in iterators:

Put ghost points into iterators, a. la

for( gp_iterator : vec)
    std::cout << *gp_iterator

prints vec[-1], vec[0], ... vec[n-1], vec[n]
where vec[-1] and vec[n] are found by gp interpolation



