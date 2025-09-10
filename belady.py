def belady_size_1_correct(access_sequence):
    cache = None  # В кеше может быть только один элемент
    hits = 0
    
    for i, current_item in enumerate(access_sequence):
        if cache == current_item:
            hits += 1  # Попадание!
            # message("HIT: элемент уже в кеше")
        else:
            # Промах - анализируем будущие запросы
            future_requests = access_sequence[i+1:]
            
            if current_item in future_requests:
                # Текущий элемент понадобится в будущем - кладем в кеш
                cache = current_item
                # message(f"MISS: кладем {current_item} в кеш (понадобится в будущем)")
            else:
                # Текущий элемент больше не понадобится - НЕ кладем в кеш!
                # Но обрабатываем запрос (отдаем пользователю)
                # message(f"MISS: обрабатываем {current_item} без помещения в кеш")
                pass  # Не меняем кеш!
    
    return hits

