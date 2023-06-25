package com.dev.FRACDN.BasicDemo.Service;

import com.dev.FRACDN.BasicDemo.Exceptions.BadRequestException;
import com.dev.FRACDN.BasicDemo.Exceptions.InternalServiceException;
import com.dev.FRACDN.BasicDemo.Exceptions.NotFoundException;
import com.dev.FRACDN.BasicDemo.Repository.BaseJpaRespositry;
import com.google.common.base.Strings;


import javax.annotation.Nonnull;
import javax.annotation.Nullable;
import java.util.function.Supplier;

/**
 * Base class for service, handles general find functions as well as some validations
 * @param <E> entity the service is for
 */
abstract class BaseService<E> {

    /**
     * Find entity by looking for the public id supplied and make sure there is something
     * @param publicId the publicId of the entity to find
     * @param repository the repository to use to find entity
     * @return entity if exists
     */
    protected E findByPublicId(@Nonnull final String publicId, @Nonnull final BaseJpaRespositry<E, Long> repository) {
        final E entity = repository.findByPublicId(publicId);
        return validateNonNullEntity(entity, () -> getFindByPublicIdError(publicId));
    }

    @Nonnull
    E validateNonNullEntity(@Nullable E entity, @Nullable final Supplier<String> messageSupplier) {
        if (entity == null) {
            if (messageSupplier != null) {
                throw new NotFoundException();
            } else {
                throw new InternalServiceException();
            }
        }
        return entity;
    }
    /**
     * The service will need to provide an error message when public id cannot be found
     *
     * @param publicId the publicId of the entity not found
     * @return an error message when entity is not found
     */
    protected abstract String getFindByPublicIdError(final String publicId);

    @SuppressWarnings("UnusedReturnValue")
    String validateNotNullPublicId(final String publicId) {
        if (Strings.isNullOrEmpty(publicId)) {
            throw new BadRequestException();
        }
        return publicId;
    }


}
