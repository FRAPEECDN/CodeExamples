package com.dev.FRACDN.BasicDemo.Repository;

import org.springframework.data.domain.Example;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.repository.NoRepositoryBean;
import org.springframework.data.repository.query.Param;

import javax.annotation.Nonnull;
import javax.annotation.Nullable;
import java.util.List;

/**
 * Generalized repository to allow general functions to support CRUD operations
 * E - will represent an Entity class
 * PK - Will be the primary key
 * * note no bean should be created for this repository
 */
@NoRepositoryBean
public interface BaseJpaRespositry<E, PK> extends JpaRepository<E, PK> {


    /**
     * Find some entities from a list of publicids supplied
     * @param publicIds list of ids to find
     * @param pageable page information for the find
     * @return page of entities
     */
    @Nullable
    Page<E> findByPublicIdIn(@Param("publicId") List<String> publicIds, @Nonnull final Pageable pageable);

    /**
     * Find a specific entity by supplying a public id
     * @param publicId specified id to find it by
     * @return optional entity
     */
    @Nullable
    E findByPublicId(@Param("publicId") String publicId);

    /**
     * Deleted a specific entity by supplying a public id
     * @param publicId specified id to find it by
     * @return deleted entity id
     */
    Long deleteByPublicId(final String publicId);

    @Override
    Page<E> findAll(@Nonnull Pageable pageable);

    @Override
    <S extends E> Page<S> findAll(@Nonnull Example<S> example, @Nonnull Pageable pageable);
}
