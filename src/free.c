#include "lib_malloc.h"

t_bool	ptr_in_page(t_page *page, void *ptr)
{
	return (t_bool) (ptr >= (void *) page + sizeof(t_page)
			&& ptr < (void *) page + SMALL_ALLOC_MULTIPLIER * g_malloc_data.pagesize);
}

t_bool	tiny_search(void *ptr)
{
	t_tiny_page *page;
	t_tiny_page *prev;

	prev = NULL;
	page = g_malloc_data.tiny_malloc_data;
	while (page && page != (ptr - (uint64_t) ptr % g_malloc_data.pagesize))
	{
		prev = page;
		page = page->next;
	}
	if (page)
	{
		tiny_free(ptr, page, prev);
		return (TRUE);
	}
	return (FALSE);
}

t_bool small_search(void *ptr)
{
	t_page *page;
	t_page *prev_page;

	prev_page = NULL;
	page = g_malloc_data.small_user_data;
	while (page && !ptr_in_page(page, ptr))
	{
		prev_page = page;
		page = page->next;
	}
	if (page == NULL)
		return (FALSE);
	small_free(ptr, page, prev_page);
	return (TRUE);
}

void    ft_free(void *ptr)
{
	if (!small_search(ptr))
		tiny_search(ptr);
}