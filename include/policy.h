/*
 * Define classes for execution dispatching
 * 
 * Idea:
 * 
 * All operations on vectors are dispatched by passing a view.
 * The view defines whether operations are executed on the host or on the device
 * 
 * This dispatching is implented by giving each view a tag:
 * 
 * class view
 * {
 *      public:
 *          using execution_tag = typename my_allocator_traits<T, allocator> :: execution_policy;
 *      ...
 * }
 *  Here execution policy is either of type host_tag or device_tag
 * 
 * 
 * For example, when passing a view to utility :: apply we do the following:
 * 
 * utility :: apply(strided_view v)
 * {
 *      detail :: impl_apply(v, v :: execution_policy)
 * }
 * 
 * 
 */

#ifndef POLICY_H
#define POLICY_H

class host_tag{};
class device_tag{};


#endif // POLICY_H